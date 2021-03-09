#include "devtools/CharacterRenderer.hpp"

// #include <filesystem>
#include <sys/stat.h>
#include "rendering/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "data/JsonAllocator.hpp"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "gason.hpp"
#include "jsonbuilder.hpp"
#include <iostream>
#include <fstream>

using namespace CR;

constexpr float RENDER_SCALE = 4.0f;

CharacterRenderer::CharacterRenderer()
{
        // if (!shader.Loaded())
        //         shader.Load("../assets/shaders/bakevert.vshader", "../assets/shaders/bakefrag.fshader");
        // shader.Bind();
        glUseProgram(0);
}

std::shared_ptr<Texture>  CharacterRenderer::RenderAnimation(const std::string& character_name, const std::string& animation_name)
{
        Character& character = GetCharacter(character_name);
        const int& frame_size = character.frame_size;

        int frame_amount = -1;
        bool frame_exists = true;
        char file_name[character_name.size() + animation_name.size() + 40];
        while (frame_exists)
        {
                frame_amount++;
                sprintf(file_name, "../dev/characters/%s/animations/%s/a_%06d.obj", character_name.c_str(), animation_name.c_str(), frame_amount);
                // std::cout << file_name << std::endl;
                // frame_exists = std::filesystem::exists(file_name);
                struct stat buffer;
                frame_exists = (stat(file_name, &buffer) == 0);
                // if (FILE* file = fopen(file_name, "r"))
                //         fclose(file);
                // else
                //         frame_exists = false;
        }
        std::cout << "rendering animation [" << animation_name << "] for character [" << character_name << "]" << std::endl;
        std::cout << "Total Frames: " << frame_amount << std::endl;

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        //make frame buffer and texture
        uint32_t frame_buffer;
        glGenFramebuffers(1, &frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        uint32_t screen_texture;
        glGenTextures(1, &screen_texture);
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        //GL_RGBA8
        // uint32_t color = 0xffffff00;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame_size * TOTAL_DIRECTIONS, frame_size * frame_amount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        uint32_t depth_render_buffer;
        glGenRenderbuffers(1, &depth_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, frame_size * TOTAL_DIRECTIONS, frame_size * frame_amount);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glClearDepth(1.0f);
        GLfloat color_clear_value[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, color_clear_value);
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);







        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "buffer error" << std::endl;
        float half_frame_size = (float)frame_size / 1.0f;
        glm::mat4 projection = glm::ortho(-RENDER_SCALE, RENDER_SCALE, -RENDER_SCALE, RENDER_SCALE, 0.1f, 30.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 vp = projection * view;
        GLint m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);



        char dir[character_name.size() + animation_name.size() + 40];
        sprintf(dir, "../dev/characters/%s/animations/%s/frame_data.json", character_name.c_str(), animation_name.c_str());
        std::shared_ptr<Json> frame_data = JsonAllocator::Get(dir);

        Shader shader;
        shader.Load("../assets/shaders/bakevert.vshader", "../assets/shaders/bakefrag.fshader");
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("vp"), 1, GL_FALSE, glm::value_ptr(vp));

        Animation& animation = character.GetAnimation(animation_name);

        if (animation.frames.size() > 0)
                animation.frames.clear();
        for (int i = 0; i < frame_amount; i++)
        {
                std::cout << "current frame: " << i << std::endl;
                animation.frames.emplace_back();

                sprintf(file_name, "../dev/characters/%s/animations/%s/a_%06d.obj", character_name.c_str(), animation_name.c_str(), i);
                objl::Loader loader;
                loader.LoadFile(file_name);
                for (int j = 0; j < TOTAL_DIRECTIONS; j++)
                {
                        // glm::mat4 model = glm::rotate(glm::pi<float>() * (1.0f / 4.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::pi<float>() * (1.0f - ((float)j * 2 / (float)TOTAL_DIRECTIONS)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(glm::vec3(frame_data->root("1")[i]("x_origin").toNumber(), 3.0f, frame_data->root("1")[i]("y_origin").toNumber()));
                        glm::mat4 model = glm::rotate(glm::pi<float>() * (1.0f / 4.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::pi<float>() * (1.0f - ((float)j * 2 / (float)TOTAL_DIRECTIONS)), glm::vec3(0.0f, 1.0f, 0.0f));
                        model *= glm::translate(glm::vec3(-frame_data->root("Origin")[i]("x").toNumber(), 3.0f, -frame_data->root("Origin")[i]("y").toNumber()));
                        // std::cout << "FACTOR: " << glm::to_string(glm::vec4(0.0f, 0.f, 0.f, 1.f) / (vp * model)) << std::endl;
                        // glm::vec2 unit = vp * glm::vec4(1.f, 0.f, 0.f, 0.f);
                        // std::cout << "(0,0): " << glm::to_string(vp * model * glm::vec4(0.f, 0.f, 0.f, 1.f)) << std::endl;
                        // std::cout << "unit: " << glm::length(unit) << std::endl;
                        glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
                        glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
                        glUniformMatrix4fv(shader.GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

                        glViewport(frame_size * j, frame_size * i, frame_size, frame_size);
                        for (objl::Mesh mesh : loader.LoadedMeshes)
                        {
                                std::vector<CharacterRenderer::Vertex> vertecies;
                                float color = atof(mesh.MeshMaterial.name.c_str()) / 255.0f;
                                // float color = 1.0f;

                                //add vertecies
                                for (objl::Vertex vertex : mesh.Vertices)
                                {
                                        vertecies.push_back({
                                                {-vertex.Position.X, vertex.Position.Y, vertex.Position.Z},
                                                // {vertex.Normal.X, -vertex.Normal.Z, -vertex.Normal.Y},
                                                {-vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z},
                                                color
                                        });
                                        // std::cout << glm::to_string(vertecies.back().position) << std::endl;
                                }



                                uint32_t vao;
                                glCreateVertexArrays(1, &vao);
                                glBindVertexArray(vao);

                                uint32_t vbo;
                                glCreateBuffers(1, &vbo);
                                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                                glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(vertecies.at(0)), vertecies.data(), GL_STATIC_DRAW);

                                glEnableVertexArrayAttrib(vao, 0);
                                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CharacterRenderer::Vertex), (const void*)offsetof(CharacterRenderer::Vertex, position));

                                glEnableVertexArrayAttrib(vao, 1);
                                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CharacterRenderer::Vertex), (const void*)offsetof(CharacterRenderer::Vertex, normal));

                                glEnableVertexArrayAttrib(vao, 2);
                                glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(CharacterRenderer::Vertex), (const void*)offsetof(CharacterRenderer::Vertex, color));


                                uint32_t ibo;
                                glGenBuffers(1, &ibo);
                                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                                glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.LoadedIndices.size() * sizeof(loader.LoadedIndices.at(0)), loader.LoadedIndices.data(), GL_STATIC_DRAW);
                                // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);

                                // glBindBuffer(GL_ARRAY_BUFFER, vbo);
                                // glBufferSubData(GL_ARRAY_BUFFER, 0, vertecies.size() * sizeof(CharacterRenderer::Vertex), vertecies.data());
                                // glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, );
                                // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                                // glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(ib), ib);

                                // // shader.GetUniformLocation()

                                // glBindVertexArray(vao);
                                glDrawElements(GL_TRIANGLES, loader.LoadedIndices.size(), GL_UNSIGNED_INT, nullptr);//(const void*)loader.LoadedIndices.data());
                                glDeleteBuffers(1, &vbo);
                                glDeleteBuffers(1, &ibo);
                                glDeleteVertexArrays(1, &vao);
                                vertecies.clear();
                        }
                }
                glfwPollEvents();
        }
        // glClear(GL_COLOR_BUFFER_BIT);

        //Save
        unsigned char* image_buffer = (unsigned char *)malloc((int)(frame_size * TOTAL_DIRECTIONS * frame_size * frame_amount * 4));
        glReadPixels(0, 0, frame_size * TOTAL_DIRECTIONS, frame_size * frame_amount, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
        char out[character_name.size() + animation_name.size() + 40];
        sprintf(out, "../dev/characters/%s/animations/%s/output.png", character_name.c_str(), animation_name.c_str());
        std::cout << "saving image to " << out << std::endl;
        stbi_write_png(out, frame_size * TOTAL_DIRECTIONS, frame_size * frame_amount, 4, image_buffer, 4 * frame_size * TOTAL_DIRECTIONS);
        std::cout << "image saved" << std::endl;
        free(image_buffer);

        //reset opengl
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteFramebuffers(1, &frame_buffer);
        glDeleteRenderbuffers(1, &depth_render_buffer);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        // glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
        glClearColor(color_clear_value[0], color_clear_value[1], color_clear_value[2], color_clear_value[3]);


        return std::make_shared<Texture>(screen_texture, frame_size * TOTAL_DIRECTIONS, frame_size * frame_amount);
}

void CharacterRenderer::ExportCharacter(const std::string& character_name)
{
        Character& character = GetCharacter(character_name);
        character.Export();
}

void Character::Export()
{
        char json_buffer[(1024 * 500) + 1] = {0};
        gason::JSonBuilder json(json_buffer, 1024 * 500);
        json.startObject();

        json.addValue("rows", TOTAL_DIRECTIONS);
        json.addValue("animation_count", animations.size());
        json.startArray("animation_names");
        for (const auto & [animation_name, animation] : animations)
        {
                json.addValue(animation_name.c_str());
        }
        json.endArray();

        int total_frames = 0;
        for (const auto & [name, animation] : animations)
        {
                total_frames += animation.frames.size();
        }
        // char* tilemap_buffer = (char *)malloc(total_frames * TOTAL_DIRECTIONS * 12);
        std::vector<uint16_t> tilemap_buffer;
        tilemap_buffer.reserve(total_frames * TOTAL_DIRECTIONS * 6);

        int counted_frames = 0;
        int current_height = 0;
        for (auto & [animation_name, animation] : animations)
        {
                //read texture
                char dir[name.size() + animation_name.size() + 40];
                sprintf(dir, "../dev/characters/%s/animations/%s/output.png", name.c_str(), animation_name.c_str());
                int w, h, comp;
                unsigned char*  image_data = stbi_load(dir, &w, &h, &comp, STBI_rgb_alpha);
                //make sure the image loaded and is the correct size
                if (image_data == NULL)
                {
                        std::cout << "image couldn't load" << std::endl;
                        continue;
                }
                if (w != frame_size * TOTAL_DIRECTIONS || h != frame_size * animation.frames.size())
                {
                        std::cout << "invalid image" << std::endl;
                        continue;
                }

                //trim transparent edges
                for (int j = 0; j < animation.frames.size(); j++)
                {
                        for (int i = 0; i < TOTAL_DIRECTIONS; i++)
                        {
                                int margin_top, margin_bot, margin_left, margin_right;
                                margin_top = margin_bot = margin_left = margin_right = 0;
                                //find top margin
                                for (int y = 0; y < frame_size; y++)
                                {
                                        bool all_transparent = true;
                                        for (int x = 0; x < frame_size; x++)
                                        {
                                                if (image_data[(i * frame_size + frame_size * TOTAL_DIRECTIONS * (frame_size * j + y) + x) * 4 + 3] != (char)0x00)
                                                {
                                                        all_transparent = false;
                                                        break;
                                                }
                                        }
                                        if (all_transparent)
                                                margin_top++;
                                        else
                                                break;
                                }
                                //find bottom margin
                                for (int y = frame_size - 1; y >= 0; y--)
                                {
                                        bool all_transparent = true;
                                        for (int x = 0; x < frame_size; x++)
                                        {
                                                if (image_data[(i * frame_size + frame_size * TOTAL_DIRECTIONS * (frame_size * j + y) + x) * 4 + 3] != (char)0x00)
                                                {
                                                        all_transparent = false;
                                                        break;
                                                }
                                        }
                                        if (all_transparent)
                                                margin_bot++;
                                        else
                                                break;
                                }
                                //find left margin
                                for (int x = 0; x < frame_size; x++)
                                {
                                        bool all_transparent = true;
                                        for (int y = margin_top; y < frame_size - margin_bot; y++)
                                        {
                                                if (image_data[(i * frame_size + frame_size * TOTAL_DIRECTIONS * (frame_size * j + y) + x) * 4 + 3] != (char)0x00)
                                                {
                                                        all_transparent = false;
                                                        break;
                                                }
                                        }
                                        if (all_transparent)
                                                margin_left++;
                                        else
                                                break;
                                }
                                //find right margin
                                for (int x = frame_size - 1; x >= 0; x--)
                                {
                                        bool all_transparent = true;
                                        for (int y = margin_top; y < frame_size - margin_bot; y++)
                                        {
                                                if (image_data[(i * frame_size + frame_size * TOTAL_DIRECTIONS * (frame_size * j + y) + x) * 4 + 3] != (char)0x00)
                                                {
                                                        all_transparent = false;
                                                        break;
                                                }
                                        }
                                        if (all_transparent)
                                                margin_right++;
                                        else
                                                break;
                                }
                                animation.frames.at(j).from_tiles.at(i).pos = glm::ivec2(i * frame_size + margin_left, j * frame_size + margin_top);
                                animation.frames.at(j).from_tiles.at(i).size = glm::ivec2(frame_size - margin_left - margin_right, frame_size - margin_top - margin_bot);
                                animation.frames.at(j).from_tiles.at(i).origin = glm::ivec2(int(frame_size * 0.5f) - margin_left, int(frame_size * 0.8f) - margin_top);
                        }
                }
                //pack animations
                glm::ivec2 offset = glm::ivec2(0.f);
                int width = 0;
                for (int i = 0; i < animation.frames.size(); i++)
                {
                        int max_height = 0;
                        offset.x = 0;
                        for (int j = 0; j < TOTAL_DIRECTIONS; j++)
                        {
                                animation.frames.at(i).tiles.at(j).pos = offset;
                                animation.frames.at(i).tiles.at(j).size = animation.frames.at(i).from_tiles.at(j).size;
                                animation.frames.at(i).tiles.at(j).origin = animation.frames.at(i).from_tiles.at(j).origin;
                                offset.x += animation.frames.at(i).from_tiles.at(j).size.x + 1;
                                int height = animation.frames.at(i).from_tiles.at(j).size.y;
                                if (height > max_height)
                                        max_height = height;
                        }
                        offset.y += max_height + 1;
                        if (offset.x > width)
                                width = offset.x;
                }
                int height = offset.y;
                animation.image_width = width;
                animation.image_height = height;
                //allocate animation texture;
                animation.animation_image_buffer = (unsigned char *)malloc(width * height * 4);
                //blit the animation texture

                for (int i = 0; i < animation.frames.size(); i++)
                {
                        for (int j = 0; j < TOTAL_DIRECTIONS; j++)
                        {
                                const Tile& tile = animation.frames.at(i).tiles.at(j);
                                const Tile& from_tile = animation.frames.at(i).from_tiles.at(j);
                                for (int y = 0; y < animation.frames.at(i).tiles.at(j).size.y; y++)
                                {
                                        memcpy(&animation.animation_image_buffer[(width * (tile.pos.y + y) + tile.pos.x) * 4],
                                                &image_data[(w * (from_tile.pos.y + y) + from_tile.pos.x) * 4],
                                                tile.size.x * 4);
                                }
                        }
                }



                stbi_image_free(image_data);
                std::cout << "animation packed: " << animation.name << std::endl;

                // //save the animation texture (for debug)
                // sprintf(dir, "../dev/characters/%s/animations/%s/packed.png", name.c_str(), animation_name.c_str());
                // std::cout << "saving packed texture..." << std::endl;
                // stbi_write_png(dir, width, height, 4, animation.animation_image_buffer, width * 4);

                //read frame data
                sprintf(dir, "../dev/characters/%s/animations/%s/frame_data.json", name.c_str(), animation_name.c_str());
                std::shared_ptr<Json> frame_data = JsonAllocator::Get(dir);

                json.startObject(animation_name.c_str());
                if (frame_data->parsed)
                {
                        json.startArray("Origin");
                        for (int i = 0; i < animation.frames.size(); i++)
                        {
                                double x = frame_data->root("Origin")[i]("x").toNumber();
                                double y = frame_data->root("Origin")[i]("y").toNumber();
                                json.startObject()
                                .addValue("x", x)
                                .addValue("y", y);
                                if (i > 0)
                                {
                                        json
                                        .addValue("dx", x - frame_data->root("Origin")[i - 1]("x").toNumber())
                                        .addValue("dy", y - frame_data->root("Origin")[i - 1]("y").toNumber());
                                }
                                else
                                {
                                        json
                                        .addValue("dx", x)
                                        .addValue("dy", y);
                                }
                                json.endObject();
                        }
                        json.endArray();
                        json.startArray("Hitboxes")
                        .startArray();

                        for (int i = 0; i < animation.frames.size(); i++)
                        {
                                json.startObject();
                                if (frame_data->root("1")[i]("scale").toNumber() > 0)
                                {
                                        json.addValue("enabled", true)
                                        .addValue("scale", frame_data->root("1")[i]("scale").toNumber())
                                        .addValue("x", frame_data->root("1")[i]("x").toNumber())
                                        .addValue("y", frame_data->root("1")[i]("y").toNumber())
                                        .addValue("x_dir", frame_data->root("1")[i]("x_dir").toNumber())
                                        .addValue("y_dir", frame_data->root("1")[i]("y_dir").toNumber())
                                        .addValue("damage", frame_data->root("1")[i]("damage").toNumber());
                                }
                                else
                                {
                                        json.addValue("enabled", false);
                                }
                                json.endObject();
                        }
                        json.endArray()
                        .endArray();
                }
                json.addValue("Tilemap Start Index", counted_frames * TOTAL_DIRECTIONS)
                .addValue("framecount", animation.frames.size())
                .addValue("fps", 60)
                .endObject();
                counted_frames += animation.frames.size();
        }





        //find width & height
        int width = 0;
        int height = 0;
        for (const auto & [name, animation] : animations)
        {
                height += animation.image_height;
                if (animation.image_width > width)
                        width = animation.image_width;
        }

        //allocate megatexture;
        unsigned char* image_buffer = (unsigned char *)malloc(width * height * 4);
        int height_offset = 0;
        for (auto & [animation_name, animation] : animations)
        {
                //add the texture to the megatexture
                for (int y = 0; y < animation.image_height; y++)
                        memcpy(&image_buffer[width * (height_offset + y) * 4], &animation.animation_image_buffer[animation.image_width * y * 4], animation.image_width * 4);
                //free the loaded texture
                for (int i = 0; i < animation.frames.size(); i++)
                {
                        for (int j = 0; j < TOTAL_DIRECTIONS; j++)
                        {
                                Tile& tile = animation.frames.at(i).tiles.at(j);
                                tile.pos.y += height_offset;
                                tilemap_buffer.emplace_back(uint16_t(tile.pos.x));
                                tilemap_buffer.emplace_back(uint16_t(tile.pos.y));
                                tilemap_buffer.emplace_back(uint16_t(tile.size.x));
                                tilemap_buffer.emplace_back(uint16_t(tile.size.y));
                                tilemap_buffer.emplace_back(uint16_t(tile.origin.x));
                                tilemap_buffer.emplace_back(uint16_t(tile.origin.y));
                        }
                }
                height_offset += animation.image_height;
        }


        //save the megatexture
        char out[name.size() + 40];
        sprintf(out, "../dev/characters/%s/%s/%s.png", name.c_str(), name.c_str(), name.c_str());
        std::cout << "saving megatexture..." << std::endl;
        stbi_write_png(out, width, height, 4, image_buffer, width * 4);
        std::cout << "saved megatexture" << std::endl;
        free(image_buffer);

        //save the tilemap
        sprintf(out, "../dev/characters/%s/%s/%s.tilemap", name.c_str(), name.c_str(), name.c_str());
        std::ofstream tilemap_file(out, std::ios::binary);
        tilemap_file.write((char*)tilemap_buffer.data(), tilemap_buffer.size() * 2);
        tilemap_file.close();

        //save the json
        json.addValue("lines", height);
        json.endObject();
        sprintf(out, "../dev/characters/%s/%s/%s.animation", name.c_str(), name.c_str(), name.c_str());
        std::ofstream json_file;
        json_file.open(out);
        json_file << json_buffer;
        json_file.close();
}

Character& CharacterRenderer::GetCharacter(const std::string& character_name)
{
        auto it = characters.find(character_name);
        if (it != characters.end())
        {
                return it->second;
        }
        else
        {
                characters.insert(std::make_pair(character_name, Character(character_name)));
                return GetCharacter(character_name);
        }
}

void CharacterRenderer::LoadCharacter(const std::string& character_name)
{
        Character& character = GetCharacter(character_name);

        // read animation data
        char dir[character_name.size() + 40];
        sprintf(dir, "../dev/characters/%s/%s/%s.animation", character_name.c_str(), character_name.c_str(), character_name.c_str());
        std::shared_ptr<Json> animation_data = JsonAllocator::Get(dir);
        std::cout << "loading character " << character_name << std::endl;
        for (int i = 0; i < animation_data->root("animation_count").toNumber(); i++)
        {
                std::string animation_name(animation_data->root("animation_names").at(i).toString());
                std::cout << "loading animation " << animation_name << std::endl;
                // character.animations.emplace(name);
                auto& anim = character.GetAnimation(animation_name);
                for (int i = 0; i < round(animation_data->root(animation_name.c_str())("framecount").toNumber()); i++)
                {
                        anim.frames.emplace_back();
                }
                anim.fps = (unsigned int)round(animation_data->root(animation_name.c_str())("fps").toNumber());
        }
        // animation_data->root("animation_names").
}

Animation& Character::GetAnimation(const std::string& animation)
{
        auto it = animations.find(animation);
        if (it != animations.end())
        {
                return it->second;
        }
        else
        {
                animations.insert(std::make_pair(animation, Animation(animation)));
                return GetAnimation(animation);
        }
}