#include "rendering/LevelRenderer.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void LevelRenderer::Init()
{
        glCreateVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glCreateBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, MaxVertexCount, nullptr, GL_STATIC_DRAW);

        glEnableVertexArrayAttrib(VAO, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(LevelRenderer::Vertex), (const void*)offsetof(LevelRenderer::Vertex, position));

        glEnableVertexArrayAttrib(VAO, 1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(LevelRenderer::Vertex), (const void*)offsetof(LevelRenderer::Vertex, texCoords));

        glEnableVertexArrayAttrib(VAO, 2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(LevelRenderer::Vertex), (const void*)offsetof(LevelRenderer::Vertex, texIndex));


        glCreateBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        glCreateTextures(GL_TEXTURE_2D, 1, &WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, WhiteTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        uint32_t color = 0xff0a0000;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

        glCreateTextures(GL_TEXTURE_2D, 1, &BlueTexture);
        glBindTexture(GL_TEXTURE_2D, BlueTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        color = 0xff303000;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

        glCreateTextures(GL_TEXTURE_2D, 1, &RedTexture);
        glBindTexture(GL_TEXTURE_2D, RedTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        color = 0xff000030;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

        glCreateTextures(GL_TEXTURE_2D, 1, &GreenTexture);
        glBindTexture(GL_TEXTURE_2D, GreenTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        color = 0xff00202a;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

        TextureSlots[0] = WhiteTexture;
        TextureSlots[1] = BlueTexture;
        TextureSlots[2] = RedTexture;
        TextureSlots[3] = GreenTexture;
        for (size_t i = 4; i < MaxTextures; i++)
                TextureSlots[i] = 0;


        // shader.Load("../assets/shaders/basicvert.vshader", "../assets/shaders/basicfrag.fshader");
        shader.Load("../assets/shaders/levelvert.vshader", "../assets/shaders/levelfrag.fshader");
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("ortho"), 1, GL_FALSE, &viewMatrix[0][0]);
        int samplers[MaxTextures];
        for (int i = 0; i < MaxTextures; i++)//figure out how to use maxTextures
        {
                samplers[i] = i;
        }
        glUniform1iv(shader.GetUniformLocation("u_Textures"), MaxTextures, samplers);
        glUseProgram(0);
}

void LevelRenderer::SetLevel(const Level& level)
{
        VA.clear();
        IA.clear();
        // VA.push_back({glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.f});
        // VA.push_back({glm::vec2(100.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.f});
        // VA.push_back({glm::vec2(0.0f, 100.0f), glm::vec2(0.0f, 0.0f), 0.f});
        // VA.push_back({glm::vec2(100.0f, 100.0f), glm::vec2(0.0f, 0.0f), 0.f});
        // IA.push_back(0);
        // IA.push_back(1);
        // IA.push_back(2);
        // IA.push_back(1);
        // IA.push_back(2);
        // IA.push_back(3);
        for (auto& section : level.level_sections)
        {
                uint32_t size = VA.size();
                for (glm::vec2 vertex : section.voronoi_boundary)
                {
                        VA.push_back({
                                vertex,
                                glm::vec2(0.f, 0.f),
                                float(section.type)
                        });
                }
                for (int i = 2; i < section.voronoi_boundary.size(); i++)
                {
                        IA.push_back(size + 0);
                        IA.push_back(size + i - 1);
                        IA.push_back(size + i);
                }
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VA.size() * sizeof(VA.at(0)), VA.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IA.size() * sizeof(IA.at(0)), IA.data(), GL_STATIC_DRAW);
}
void LevelRenderer::Render()
{
        shader.Bind();
        for (uint32_t i = 0; i < TextureSlotIndex; i++)
        {
                glBindTextureUnit(i, TextureSlots[i]);
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, IA.size(), GL_UNSIGNED_INT, nullptr);
}
void LevelRenderer::SetViewMatrix(glm::mat4 vm)
{
        viewMatrix = vm;
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("ortho"), 1, GL_FALSE, &viewMatrix[0][0]);
}