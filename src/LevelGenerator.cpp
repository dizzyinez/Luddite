#include "level/LevelGenerator.hpp"
#include <iostream>
#include <thinks/poisson_disk_sampling.h>
// #include <delaunator.hpp>
#include <bitset>
#include <limits>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#define JC_VORONOI_IMPLEMENTATION
#include <jc_voronoi/jc_voronoi.h>
#include <jc_voronoi/jc_voronoi_clip.h>


//TODO: template this function for total types
Level LevelGenerator::Generate(uint32_t seed)
{
        constexpr float level_size = 2000.f;
        //Generate Poisson Disk Samples
        constexpr float max_radius = 900.f;
        constexpr std::array<double, 2> kXMin = {-level_size, -level_size};
        constexpr std::array<double, 2> kXMax = {level_size, level_size};
        const std::vector<std::array<double, 2> > samples = thinks::PoissonDiskSampling(350.d, kXMin, kXMax, 30, 1);
        const int total_points = samples.size();

        constexpr uint8_t total_types = 4;
        std::array<std::bitset<total_types>, total_types> valid_neighbors;
        valid_neighbors = {0b1110, 0b1101, 0b1011, 0b0111};
        // valid_neighbors = {0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111};

        struct Point
        {
                Point() = default;
                Point(glm::vec2 position_) : position(position_) {}
                glm::vec2 position;
                std::vector<uint32_t> neighbor_indicies;
                std::bitset<total_types> possible_types = std::move(std::bitset<total_types>{}.set()); //set all to 1
                int16_t type = -1;
        };
        // Point propogator[total_points];
        std::vector<Point> propogator;
        propogator.reserve(total_points);


        jcv_point points[total_points];
        for (int i = 0; i < total_points; i++)
        {
                propogator.emplace_back();
                points[i] = {samples[i][0], samples[i][1]};
        }

        jcv_diagram diagram;
        memset(&diagram, 0, sizeof(jcv_diagram));
        //TODO: PASS JVC_RECT
        // jcv_rect rect = {{-level_size, -level_size}, {level_size, level_size}};
        jcv_diagram_generate(total_points, points, 0, 0, &diagram);
        {
                const jcv_site* sites = jcv_diagram_get_sites(&diagram);
                for (int i = 0; i < diagram.numsites; ++i)
                {
                        const jcv_site* site = &sites[i];
                        propogator[site->index].position = glm::vec2(site->p.x, site->p.y);
                }

                const jcv_edge* edge = jcv_diagram_get_edges(&diagram);
                while (edge)
                {
                        if (edge->sites[0] && edge->sites[1])
                        {
                                propogator[edge->sites[0]->index].neighbor_indicies.emplace_back(edge->sites[1]->index);
                                propogator[edge->sites[1]->index].neighbor_indicies.emplace_back(edge->sites[0]->index);
                        }
                        edge = jcv_diagram_get_next_edge(edge);
                }
        }
        // std::vector<double> coords;
        // coords.reserve(samples.size() * 2);
        // for (auto sample : samples)
        // {
        //         for (double f : sample)
        //         {
        //                 coords.push_back(f);
        //         }
        // }
        //Generate a Delaunay triangulation from the samples
        // delaunator::Delaunator d(coords);

        // bool collapsed = false;
        //create a temporary propogator
        // std::vector<Point> temp_propogator(propogator);
        struct Propogation
        {
                static bool Propogate(std::vector<Point>& propogator, uint32_t index, uint8_t type, const std::array<std::bitset<total_types>, total_types>& valid_neighbors)
                {
                        propogator[index].possible_types.reset();
                        propogator[index].type = type;
                        bool conflict = false;
                        for (uint32_t neighbor : propogator.at(index).neighbor_indicies)
                        {
                                if (propogator[neighbor].possible_types.any())
                                {
                                        propogator[neighbor].possible_types &= valid_neighbors.at(type);
                                        //if neighbor has 0 valid types, backtrack and invalidate the chosen type to the propogator
                                        if (propogator[neighbor].possible_types.none())
                                        {
                                                conflict = true;
                                                break;
                                        }
                                }
                        }
                        for (uint32_t neighbor : propogator.at(index).neighbor_indicies)
                        {
                                if (propogator[neighbor].possible_types.count() == 1)
                                {
                                        uint8_t new_type;
                                        for (uint8_t i = 0; i < total_types; i++)
                                        {
                                                if (propogator[neighbor].possible_types[i] == 1)
                                                {
                                                        new_type = i;
                                                        break;
                                                }
                                        }
                                        std::vector<Point> temp_propogator(propogator);
                                        if (Propogate(temp_propogator, neighbor, new_type, valid_neighbors))
                                        {
                                                conflict = true;
                                                break;
                                        }
                                        else
                                        {
                                                propogator = temp_propogator;
                                        }
                                }
                        }
                        return conflict;

                        // if (conflict)
                        // {
                        //         std::cout << "CONFLICT" << std::endl;
                        //         // std::cout << "conflict! Point " << (int)new_point << " can't be type " << (int)new_type << std::endl;
                        //         //PROBLEM IS HERE
                        //         //some points will just not have any viable options and will just cause a conflict and then have no possible types themselves
                        //         //so the conflict checking needs to go deeper
                        //         //make it go back to the most recent "guess", which was a choice when there was more than one possible type
                        //         propogator[index].possible_types.reset(type);
                        //         temp_propogator = propogator;
                        // }
                        // else
                        // {
                        //         propogator = temp_propogator;
                        // }
                }

                static bool Tree(std::vector<Point>& propogator, const std::array<std::bitset<total_types>, total_types>& valid_neighbors)
                {
                        std::vector<uint32_t> lowest_points;
                        uint8_t lowest_entropy = total_types;
                        bool uncollapsed_point = false;
                        bool undefined_point = false;
                        int total_uncollapsed = 0;
                        int total_entropy = 0;
                        for (uint32_t i = 0; i < propogator.size(); i++)
                        {
                                uint8_t entropy = propogator[i].possible_types.count();
                                if (entropy > 0 && entropy <= lowest_entropy)
                                {
                                        total_entropy += entropy;
                                        total_uncollapsed++;
                                        uncollapsed_point = true;
                                        if (entropy < lowest_entropy)
                                                lowest_points.clear();
                                        lowest_points.emplace_back(i);
                                }
                                if (propogator[i].type == -1)
                                {
                                        undefined_point = true;
                                }
                        }
                        if (!uncollapsed_point)
                        {
                                if (undefined_point)
                                {
                                        return false;
                                }
                                return true;
                                std::cout << "returning true" << std::endl;
                        }
                        uint32_t new_point = lowest_points.at(0);
                        //select random from lowest
                        std::cout << total_entropy << "\n";

                        std::vector<uint8_t> possible_types;
                        for (uint8_t i = 0; i < total_types; i++)
                        {
                                if (propogator[new_point].possible_types[i] == 1)
                                        possible_types.emplace_back(i);
                        }
                        uint8_t new_type = possible_types.at(0);


                        std::vector<Point> temp_propogator(propogator);
                        bool conflict = Propogation::Propogate(temp_propogator, new_point, new_type, valid_neighbors);
                        if (conflict)
                        {
                                // std::cout << "CONFLICT" << std::endl;
                                propogator[new_point].possible_types.reset(new_type);
                                return Tree(propogator, valid_neighbors);
                        }
                        else
                        {
                                if (Tree(temp_propogator, valid_neighbors))
                                {
                                        propogator = temp_propogator;
                                        return true;
                                }
                                else
                                {
                                        // std::cout << "MEGACONFLICT" << std::endl;
                                        propogator[new_point].possible_types.reset(new_type);
                                        return Tree(propogator, valid_neighbors);
                                }
                        }
                }
        };

        bool level_possible = Propogation::Tree(propogator, valid_neighbors);
        if (level_possible)
                std::cout << "done! Level is possible" << std::endl;
        else
                std::cout << "Level is NOT possible" << std::endl;
        Level level;
        for (Point point : propogator)
        {
                auto& section = level.level_sections.emplace_back(glm::vec2(point.position[0], point.position[1]), point.type);
                section.neighbor_section_indicies = point.neighbor_indicies;
        }
        {
                const jcv_site* sites = jcv_diagram_get_sites(&diagram);
                for (int i = 0; i < diagram.numsites; ++i)
                {
                        const jcv_site* site = &sites[i];
                        const jcv_graphedge* e = site->edges;
                        bool next_edge = true;
                        while (e)
                        {
                                level.level_sections[site->index].voronoi_boundary.push_back(glm::vec2(e->pos->x, e->pos->y));
                                // if (e->next == nullptr)
                                //         next_edge = false;
                                // else
                                e = e->next;
                        }
                }
        }



        //sort vertecies clockwise
        for (auto& section : level.level_sections)
        {
                std::sort(section.voronoi_boundary.begin(), section.voronoi_boundary.end(), [&section](const glm::vec2& section1, const glm::vec2& section2) {
                        glm::vec2 vec1 = glm::normalize(section1 - section.center);
                        glm::vec2 vec2 = glm::normalize(section2 - section.center);
                        return glm::atan(vec1.x, vec1.y) < glm::atan(vec2.x, vec2.y);
                });
        }
        jcv_diagram_free(&diagram);
        return level;
}