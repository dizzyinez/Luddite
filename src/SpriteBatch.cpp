#include "rendering/SpriteBatch.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include "stb/stb_image.h"

void SpriteBatch::Clean()
{
        glDeleteVertexArrays(1, &QuadVA);
        glDeleteBuffers(1, &QuadVB);
        glDeleteBuffers(1, &QuadIB);

        glDeleteTextures(1, &PalleteTexture);

        delete[] QuadBuffer;
}

void SpriteBatch::BeginBatch()
{
        QuadBufferPtr = QuadBuffer;
}

void SpriteBatch::EndBatch()
{
        GLsizeiptr size = (uint8_t*)QuadBufferPtr - (uint8_t*)QuadBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, QuadVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, QuadBuffer);
}

void SpriteBatch::Flush()
{
        shader.Bind();
        for (uint32_t i = 0; i < TextureSlotIndex; i++)
        {
                glBindTextureUnit(i, TextureSlots[i]);
        }
        glBindVertexArray(QuadVA);
        glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);

        IndexCount = 0;
        TextureSlotIndex = 1;
}


void SpriteBatch::Init()
{
        MaxQuadCount = 5000;
        MaxVertexCount = MaxQuadCount * 4;
        MaxIndexCount = MaxQuadCount * 6;

        if (QuadBuffer != nullptr)
        {
                std::cout << "Initialized a Batch multiple times!" << std::endl;
                //TODO: assert or crash or whatever
        }

        QuadBuffer = new Vertex[MaxQuadCount];

        glCreateVertexArrays(1, &QuadVA);
        glBindVertexArray(QuadVA);

        glCreateBuffers(1, &QuadVB);
        glBindBuffer(GL_ARRAY_BUFFER, QuadVB);
        glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexArrayAttrib(QuadVA, 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

        glEnableVertexArrayAttrib(QuadVA, 1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

        glEnableVertexArrayAttrib(QuadVA, 2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));

        glEnableVertexArrayAttrib(QuadVA, 3);
        glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(Vertex), (const void*)offsetof(Vertex, colors));

        uint32_t indices[MaxIndexCount];
        uint32_t offset = 0;
        for (int i = 0; i < MaxIndexCount; i += 6)
        {
                indices[i + 0] = 0 + offset;
                indices[i + 1] = 1 + offset;
                indices[i + 2] = 2 + offset;

                indices[i + 3] = 2 + offset;
                indices[i + 4] = 3 + offset;
                indices[i + 5] = 0 + offset;

                offset += 4;
        }

        glCreateBuffers(1, &QuadIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QuadIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


        int width, height, nrChannels;
        glCreateTextures(GL_TEXTURE_2D, 1, &PalleteTexture);
        glBindTexture(GL_TEXTURE_2D, PalleteTexture);
        unsigned char *data = stbi_load("../assets/textures/pallete.png", &width, &height, &nrChannels, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        TextureSlots[0] = PalleteTexture;
        for (size_t i = 1; i < MaxTextures; i++)
        {
                TextureSlots[i] = 0;
        }

        shader.Load("../assets/shaders/spritevert.vshader", "../assets/shaders/spritefrag.fshader");
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("ortho"), 1, GL_FALSE, &viewMatrix[0][0]);
        int samplers[32];
        for (int i = 0; i < 32; i++)//figure out how to use maxTextures
        {
                samplers[i] = i;
        }
        glUniform1iv(shader.GetUniformLocation("u_Textures"), 32, samplers);
}


void SpriteBatch::DrawQuad(const glm::vec2& position, const glm::vec2& size) // add colored quad
{
        std::cout << "ERROR: tried to render a sprite without providing a texture!" << std::endl;
        // if (IndexCount >= MaxIndexCount)
        // {
        //         std::cout << "Batch overflowed, beginning a new batch" << std::endl;
        //         EndBatch();
        //         Flush();
        //         BeginBatch();
        // }
        // addQuadToBuffer(position, size, 0.0f);
}

void SpriteBatch::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID, const glm::uvec4& colors)
{
        if (IndexCount >= MaxIndexCount)
        {
                std::cout << "Batch overflowed, beginning a new batch" << std::endl;
                EndBatch();
                Flush();
                BeginBatch();
        }
        //constexpr glm::vec4 color = {}

        float textureIndex = 0;
        for (uint32_t i = 1; i < TextureSlotIndex; i++)
        {
                if (TextureSlots[i] == textureID)
                {
                        textureIndex = (float)i;
                        break;
                }
        }

        if (textureIndex == 0)//TODO: check if above maximum textures
        {
                textureIndex = (float)TextureSlotIndex;
                TextureSlots[TextureSlotIndex] = textureID;
                TextureSlotIndex++;
        }

        // std::cout << textureIndex << std::endl;
        addQuadToBuffer(position, size, textureIndex, colors);
}



void SpriteBatch::addQuadToBuffer(const glm::vec2& position, const glm::vec2& size, float texIndex, const glm::uvec4& colors)
{
        QuadBufferPtr->position = {position.x, position.y, 0.0f};
        QuadBufferPtr->texCoords = {0.0f, 0.0f};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr->colors = colors;
        QuadBufferPtr++;

        QuadBufferPtr->position = {position.x + size.x, position.y, 0.0f};
        QuadBufferPtr->texCoords = {1.0f, 0.0f};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr->colors = colors;
        QuadBufferPtr++;

        QuadBufferPtr->position = {position.x + size.x, position.y + size.y, 0.0f};
        QuadBufferPtr->texCoords = {1.0f, 1.0f};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr->colors = colors;
        QuadBufferPtr++;

        QuadBufferPtr->position = {position.x, position.y + size.y, 0.0f};
        QuadBufferPtr->texCoords = {0.0f, 1.0f};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr->colors = colors;
        // std::cout << QuadBufferPtr->colors.x << std::endl;
        QuadBufferPtr++;

        IndexCount += 6;
}

void SpriteBatch::SetViewMatrix(glm::mat4 &vm)
{
        viewMatrix = vm;
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("ortho"), 1, GL_FALSE, &viewMatrix[0][0]);
}
