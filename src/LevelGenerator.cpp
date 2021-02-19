#include "level/LevelGenerator.hpp"
#include <iostream>
#include <poisson_disk_sampling.h>
#include <delaunator.hpp>
#include <bitset>
#include <limits>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>
#define JC_VORONOI_IMPLEMENTATION
#include <jc_voronoi.h>
#include <jc_voronoi_clip.h>


//TODO: template this function for total types
Level LevelGenerator::Generate(uint32_t seed)
{
        //Generate Poisson Disk Samples
        constexpr float max_radius = 900.f;
        constexpr std::array<double, 2> kXMin = {-500.F, -500.F};
        constexpr std::array<double, 2> kXMax = {500.F, 500.F};
        const std::vector<std::array<double, 2> > samples = thinks::PoissonDiskSampling(50.d, kXMin, kXMax, 30, 0);
        std::vector<double> coords;
        coords.reserve(samples.size() * 2);
        for (auto sample : samples)
        {
                for (double f : sample)
                {
                        coords.push_back(f);
                }
        }

        //Generate a Delaunay triangulation from the samples
        delaunator::Delaunator d(coords);
        // for (std::size_t i = 0; i < d.triangles.size(); i += 3)
        // {
        //         printf(
        //                 "Triangle points: [[%f, %f], [%f, %f], [%f, %f]]\n",
        //                 d.coords[2 * d.triangles[i]], //tx0
        //                 d.coords[2 * d.triangles[i] + 1], //ty0
        //                 d.coords[2 * d.triangles[i + 1]], //tx1
        //                 d.coords[2 * d.triangles[i + 1] + 1],//ty1
        //                 d.coords[2 * d.triangles[i + 2]], //tx2
        //                 d.coords[2 * d.triangles[i + 2] + 1] //ty2
        //                 );
        // }


        constexpr uint8_t total_types = 4;
        std::array<std::bitset<total_types>, total_types> valid_neighbors;
        valid_neighbors = {0b1111, 0b1101, 0b1011, 0b0111};
        // valid_neighbors = {0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111};

        //create a list of points and their neighbors using the Delaunay triangulation
        struct Point
        {
                Point(std::array<double, 2> position_) : position(position_) {}
                std::array<double, 2> position;
                std::vector<uint32_t> neighbor_indicies;
                std::bitset<total_types> possible_types = std::move(std::bitset<total_types>{}.set()); //set all to 1
                uint8_t type;
        };
        std::vector<Point> Propogator;
        Propogator.reserve(samples.size());
        for (auto sample : samples)
        {
                Propogator.emplace_back(sample);
        }
        for (int e = 0; e < d.triangles.size(); e++)
        {
                if (e > d.halfedges.at(e))
                {
                        uint32_t point1 = d.triangles.at(e);
                        uint32_t point2 = d.triangles.at((e % 3 == 2) ? e - 2 : e + 1);
                        Propogator.at(point1).neighbor_indicies.push_back(point2);
                        Propogator.at(point2).neighbor_indicies.push_back(point1);
                }
                //Make all of the outer edges walls
                if (d.halfedges.at(e) == -1)
                {
                        // Propogator.at(d.triangles.at(e)).possible_types.reset();
                        // Propogator.at(d.triangles.at(e)).possible_types.set(0);
                }
        }


        //IMPORTANT
        // for (uint32_t d.triangles)


        // bool collapsed = false;
        //create a temporary propogator
        std::vector<Point> Temp_Propogator(Propogator);
        while (true)
        {
                //Observe
                //find set of points with lowest entropy
                std::vector<uint32_t> lowest_points;
                uint8_t lowest_entropy = total_types;
                bool uncollapsed_point = false;
                int total_uncollapsed = 0;
                for (uint32_t i = 0; i < Propogator.size(); i++)
                {
                        uint8_t entropy = Propogator.at(i).possible_types.count();
                        if (entropy > 0 && entropy <= lowest_entropy)
                        {
                                uncollapsed_point = true;
                                total_uncollapsed++;
                                if (entropy < lowest_entropy)
                                        lowest_points.clear();
                                lowest_points.emplace_back(i);
                        }
                }
                std::cout << total_uncollapsed << std::endl;
                if (!uncollapsed_point)
                {
                        //everything is collapsed
                        break;
                }

                //select random from lowest
                uint32_t new_point = lowest_points.at(0);

                //Propogate
                //collapse that point to a random set of possible types
                //  choose a random type
                std::vector<uint8_t> possible_types;
                for (uint8_t i = 0; i < total_types; i++)
                {
                        if (Propogator.at(new_point).possible_types[i] == 1)
                                possible_types.emplace_back(i);
                }
                uint8_t new_type = possible_types.at(0);
                // std::cout << "new type " << (int)new_type << std::endl;
                Temp_Propogator.at(new_point).possible_types.reset();
                Temp_Propogator.at(new_point).type = new_type;
                // Temp_Propogator.at(new_point).possible_types.set(new_type);
                //propogate valid neighbors to neighbors
                bool conflict = false;
                for (uint32_t neighbor : Temp_Propogator.at(new_point).neighbor_indicies)
                {
                        if (Temp_Propogator.at(neighbor).possible_types.any())
                        {
                                Temp_Propogator.at(neighbor).possible_types &= valid_neighbors.at(new_type);
                                //if neighbor has 0 valid types, backtrack and invalidate the chosen type to the propogator
                                if (Temp_Propogator.at(neighbor).possible_types.none())
                                {
                                        conflict = true;
                                        break;
                                }
                        }
                }

                if (conflict)
                {
                        std::cout << "CONFLICT" << std::endl;
                        // std::cout << "conflict! Point " << (int)new_point << " can't be type " << (int)new_type << std::endl;
                        //PROBLEM IS HERE
                        //some points will just not have any viable optinos and will just cause a conflict and then have no possible types themselves
                        //so the conflict checking needs to go deeper
                        //make it go back to the most recent "guess", which was a choice when there was more than one possible type
                        Propogator.at(new_point).possible_types.reset(new_type);
                        Temp_Propogator = Propogator;
                }
                else
                {
                        Propogator = Temp_Propogator;
                }
        }
        std::cout << "done!" << std::endl;
        Level level;
        for (Point point : Propogator)
        {
                auto& section = level.level_sections.emplace_back(glm::vec2(point.position[0], point.position[1]), point.type);
                section.neighbor_section_indicies = point.neighbor_indicies;
        }





        /*
         * //loop through each triangle
         * //calculate it's circumcenter
         * //add that circumcenter to each vertex's list
         * std::vector<glm::vec2> circumcenters;
         * for (int e = 0; e < d.triangles.size() / 3; e++)
         * {
         *      std::pair<double, double> center_pair = delaunator::circumcenter(
         *              d.coords.at(2 * d.triangles.at((3 * e) + 0)),
         *              d.coords.at(2 * d.triangles.at((3 * e) + 0) + 1),
         *              d.coords.at(2 * d.triangles.at((3 * e) + 1)),
         *              d.coords.at(2 * d.triangles.at((3 * e) + 1) + 1),
         *              d.coords.at(2 * d.triangles.at((3 * e) + 2)),
         *              d.coords.at(2 * d.triangles.at((3 * e) + 2) + 1));
         *      glm::vec2 center(center_pair.first, center_pair.second);
         *      float buffer = 15750.f;
         *      // if (center.x > kXMax[0] + buffer || center.x < kXMin[0] - buffer || center.y > kXMax[1] + buffer || center.y < kXMin[1] - buffer)
         *      //         break;
         *      level.level_sections.at(d.triangles.at((3 * e) + 0)).voronoi_boundary.emplace_back(center);
         *      level.level_sections.at(d.triangles.at((3 * e) + 1)).voronoi_boundary.emplace_back(center);
         *      level.level_sections.at(d.triangles.at((3 * e) + 2)).voronoi_boundary.emplace_back(center);
         *      //                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^ this is beyond sexy >B)
         *      circumcenters.push_back(center);
         *      // std::cout << glm::to_string(center) << std::endl;
         * }
         *
         *
         * //find furthest point from 0,0 -> (d)
         * // double furthest_distance_from_center_squared = std::numeric_limits<double>::max();
         * double furthest_distance_from_center_squared = 0;
         * for (auto center : circumcenters)
         * {
         *      double distance_squared = glm::length2(center);
         *      if (distance_squared > furthest_distance_from_center_squared)
         *              furthest_distance_from_center_squared = distance_squared;
         * }
         * //extra buffer for good measure
         * furthest_distance_from_center_squared += 2.d;
         * std::cout << "radius " << furthest_distance_from_center_squared << std::endl;
         * std::vector<uint32_t> edge_level_sections;
         * //loop through points on the edge
         * for (uint32_t e = 0; e < d.triangles.size(); e++)
         * {
         *      if (d.halfedges.at(e) == -1)
         *      {
         *              edge_level_sections.push_back(d.triangles.at(e));
         *              // std::cout << e << " " << d.triangles.at(e) << std::endl;
         *      }
         * }
         * //for each of the adjecent edge points, calculate the perpendicular bisector, and add the point that's (d) far from 0,0
         * // //remove edges!
         * // // sort them in reverse
         * // std::sort(edge_level_sections.begin(), edge_level_sections.end(), [](const uint32_t& a, const uint32_t& b) {return a > b;});
         * for (uint32_t e : edge_level_sections)
         * {
         *      //do next edge thing
         *      LevelSection& section = level.level_sections.at(e);
         *      section.type = 2;
         *      for (uint32_t n : section.neighbor_section_indicies)
         *      {
         *              if (std::find(edge_level_sections.begin(), edge_level_sections.end(), n) != edge_level_sections.end())
         *              {
         *                      std::cout << "edge" << std::endl;
         *                      LevelSection& neighbor = level.level_sections.at(n);
         *                      //calculate center
         *                      glm::vec2 center = (section.center + neighbor.center) * 0.5f;
         *                      glm::vec2 tangent = section.center - neighbor.center;
         *                      glm::vec2 normal = glm::normalize(glm::vec2(-tangent.y, tangent.x));
         *
         *                      //  //flip the normal if it's facing towards 0,0
         *                      //  glm::vec2 temp_dir = center + normal;
         *                      //   if ((temp_dir.x * temp_dir.x) + (temp_dir.y * temp_dir.y) < (center.x * center.x) + (center.y * center.y))
         *                      //        normal = -normal;
         *
         *                      double term_1 = (center.x * normal.x) + (center.y * normal.y);
         *                      // calculates the distance of the vector to the edge of the circle
         *                      normal *= glm::sqrt(furthest_distance_from_center_squared - (center.x * center.x) - (center.y * center.y) + (term_1 * term_1)) - term_1;
         *                      section.voronoi_boundary.push_back(center);
         *                      std::cout << glm::to_string(center) << std::endl;
         *              }
         *      }
         * }
         * //add a few points in between those two on the circle's perimeter
         */

        //sort vertecies clockwise
        for (auto& section : level.level_sections)
        {
                std::sort(section.voronoi_boundary.begin(), section.voronoi_boundary.end(), [&section](const glm::vec2& section1, const glm::vec2& section2) {
                        glm::vec2 vec1 = glm::normalize(section1 - section.center);
                        glm::vec2 vec2 = glm::normalize(section2 - section.center);
                        return glm::atan(vec1.x, vec1.y) < glm::atan(vec2.x, vec2.y);
                });
        }
        return level;
}