#include "devtools/CharacterRenderer.hpp"

// #include <filesystem>
#include <sys/stat.h>
#include "rendering/Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

// #define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "gason.hpp"
#include "jsonbuilder.hpp"

using namespace CR;

CharacterRenderer::CharacterRenderer()
{
        // if (!shader.Loaded())
        //         shader.Load("../assets/shaders/bakevert.vshader", "../assets/shaders/bakefrag.fshader");
        // shader.Bind();
        // glUniformMatrix4fv(shader.GetUniformLocation("mvp"), 1, GL_FALSE, &mvp[0][0]);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame_size * frame_amount, frame_size * 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        uint32_t depth_render_buffer;
        glGenRenderbuffers(1, &depth_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, frame_size * frame_amount, frame_size * 8);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        // glDrawBuffers(1, DrawBuffers);

        // glBindTexture(GL_TEXTURE_2D, 0);
        // glEnable(GL_TEXTURE_2D);

        // glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        glClearDepth(1.0f);
        GLfloat color_clear_value[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, color_clear_value);
        glClearColor(1, 1, 1, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);

        // glDisable(GL_BLEND);
        // glDepthFunc(GL_LEQUAL);
        // glDisable(GL_CULL_FACE);
        // glEnable(GL_CULL_FACE);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);






        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "buffer error" << std::endl;
        float half_frame_size = (float)frame_size / 1.0f;
        float scale = 3.0f;
        glm::mat4 projection = glm::ortho(-scale, scale, -scale, scale, 0.1f, 30.0f);
        // glm::mat4 projection = glm::perspective(glm::quarter_pi<float>() / 2.0f, 1.0f, 0.1f, 20.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 mvp = projection * view;
        GLint m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);




        Shader shader;
        shader.Load("../assets/shaders/bakevert.vshader", "../assets/shaders/bakefrag.fshader");
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

        Animation& animation = character.GetAnimation(animation_name);

        for (int i = 0; i < frame_amount; i++)
        {
                animation.frames.emplace_back();

                sprintf(file_name, "../dev/characters/%s/animations/%s/a_%06d.obj", character_name.c_str(), animation_name.c_str(), i);
                objl::Loader loader;
                loader.LoadFile(file_name);
                for (int j = 0; j < 8; j++)
                {
                        glm::mat4 model = glm::rotate(glm::pi<float>() * (1.0f / 4.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::pi<float>() * (1.0f - ((float)j / 4.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
                        glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
                        glUniformMatrix4fv(shader.GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
                        glUniformMatrix4fv(shader.GetUniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));

                        glViewport(frame_size * i, frame_size * j, frame_size, frame_size);
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
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                char text_to_display[10];
                sprintf(text_to_display, "Rendering Frame %d", i);
                glfwPollEvents();
                glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        }
        // glClear(GL_COLOR_BUFFER_BIT);

        //Save
        unsigned char* image_buffer = (unsigned char *)malloc((int)(frame_size * frame_amount * frame_size * 8 * 4));
        glReadPixels(0, 0, frame_size * frame_amount, frame_size * 8, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
        char out[character_name.size() + animation_name.size() + 40];
        sprintf(out, "../dev/characters/%s/animations/%s/output.png", character_name.c_str(), animation_name.c_str());
        stbi_write_png(out, frame_size * frame_amount, frame_size * 8, 4, image_buffer, 4 * frame_size * frame_amount);
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


        return std::make_shared<Texture>(screen_texture, frame_size * frame_amount, frame_size);
}

void CharacterRenderer::ExportCharacter(const std::string& character_name)
{
        Character& character = GetCharacter(character_name);
        character.Export();
}

void Character::Export()
{
        char json_buffer[(1024 * 10) + 1] = {0};
        gason::JSonBuilder json(json_buffer, 1024 * 10);
        json.startObject();

        //find widest animation;
        int width = 0;
        for (const auto & [name, animation] : animations)
                width = (animation.frames.size() > width) ? animation.frames.size() : width;
        //allocate megatexture;
        unsigned char* image_buffer = (unsigned char *)malloc((int)(frame_size * width * frame_size * 8 * 4 * animations.size()));

        int animations_counted = 0;
        for (const auto & [animation_name, animation] : animations)
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
                if (w != frame_size * animation.frames.size() || h != frame_size * 8)
                {
                        std::cout << "invalid image" << std::endl;
                        continue;
                }
                //add the texture to the megatexture
                for (/*each line*/ int i = 0; i < h; i++)
                {
                        memcpy(&image_buffer[(width * frame_size * animations_counted * frame_size * 8 * 4) + width * frame_size * i * 4], &image_data[w * 4 * i], w * 4);
                }
                //free the loaded texture
                stbi_image_free(image_data);



                // stbi_write_png(out, frame_size * frame_amount, frame_size * 8, 4, image_buffer, 4 * frame_size * frame_amount);

                // stbi_load()
                json.startObject(name.c_str())
                .addValue("framecount", animation.frames.size())
                .addValue("fps", 60)
                .addValue("line", animations_counted * 8);
                // .startArray("frames");
                // for (Frame frame : animation.frames)
                // {
                //         json.startObject();
                // }
                json.endArray();
                json.endObject();
                animations_counted++;
        }

        //save the megatexture
        char out[name.size() + 40];
        sprintf(out, "../dev/characters/%s/%s/%s.png", name.c_str(), name.c_str(), name.c_str());
        stbi_write_png(out, frame_size * width, frame_size * 8 * animations.size(), 4, image_buffer, 4 * frame_size * width);
        std::cout << "saved image" << std::endl;
        free(image_buffer);
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