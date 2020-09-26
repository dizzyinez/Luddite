#include "rendering/TextureBatch.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>


void TextureBatch::Clean()
{
        glDeleteVertexArrays(1, &QuadVA);
        glDeleteBuffers(1, &QuadVB);
        glDeleteBuffers(1, &QuadIB);

        glDeleteTextures(1, &WhiteTexture);

        delete[] QuadBuffer;
}

void TextureBatch::BeginBatch()
{
        QuadBufferPtr = QuadBuffer;
}

void TextureBatch::EndBatch()
{
        GLsizeiptr size = (uint8_t*)QuadBufferPtr - (uint8_t*)QuadBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, QuadVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, QuadBuffer);
}

void TextureBatch::Flush()
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


void TextureBatch::Init()
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
        glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(TextureBatch::Vertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexArrayAttrib(QuadVA, 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureBatch::Vertex), (const void*)offsetof(TextureBatch::Vertex, position));

        glEnableVertexArrayAttrib(QuadVA, 1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureBatch::Vertex), (const void*)offsetof(TextureBatch::Vertex, texCoords));

        glEnableVertexArrayAttrib(QuadVA, 2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(TextureBatch::Vertex), (const void*)offsetof(TextureBatch::Vertex, texIndex));

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


        glCreateTextures(GL_TEXTURE_2D, 1, &WhiteTexture);
        glBindTexture(GL_TEXTURE_2D, WhiteTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        uint32_t color = 0xffffffff;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

        TextureSlots[0] = WhiteTexture;
        for (size_t i = 1; i < MaxTextures; i++)
                TextureSlots[i] = 0;

        shader.Load("../assets/shaders/basicvert.vshader", "../assets/shaders/basicfrag.fshader");
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("ortho"), 1, GL_FALSE, &viewMatrix[0][0]);
        int samplers[32];
        for (int i = 0; i < 32; i++)//figure out how to use maxTextures
        {
                samplers[i] = i;
        }
        glUniform1iv(shader.GetUniformLocation("u_Textures"), 32, samplers);
}


void TextureBatch::DrawQuad(const glm::vec2& position, const glm::vec2& size) // add colored quad
{
        if (IndexCount >= MaxIndexCount)
        {
                std::cout << "Batch overflowed, beginning a new batch" << std::endl;
                EndBatch();
                Flush();
                BeginBatch();
        }
        addQuadToBuffer(position, size, 0.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void TextureBatch::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID)
{
        if (IndexCount >= MaxIndexCount)
        {
                std::cout << "Batch overflowed, beginning a new batch" << std::endl;
                EndBatch();
                Flush();
                BeginBatch();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < TextureSlotIndex; i++)
        {
                if (TextureSlots[i] == textureID)
                {
                        textureIndex = (float)i;
                        break;
                }
        }

        if (textureIndex == 0.0f)//TODO: check if above maximum textures
        {
                textureIndex = (float)TextureSlotIndex;
                TextureSlots[TextureSlotIndex] = textureID;
                TextureSlotIndex++;
        }

        addQuadToBuffer(position, size, textureIndex, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
}

void TextureBatch::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID, const glm::vec4& tex_coords)
{
        if (IndexCount >= MaxIndexCount)
        {
                std::cout << "Batch overflowed, beginning a new batch" << std::endl;
                EndBatch();
                Flush();
                BeginBatch();
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < TextureSlotIndex; i++)
        {
                if (TextureSlots[i] == textureID)
                {
                        textureIndex = (float)i;
                        break;
                }
        }

        if (textureIndex == 0.0f)//TODO: check if above maximum textures
        {
                textureIndex = (float)TextureSlotIndex;
                TextureSlots[TextureSlotIndex] = textureID;
                TextureSlotIndex++;
        }

        addQuadToBuffer(position, size, textureIndex, tex_coords);
}



void TextureBatch::addQuadToBuffer(const glm::vec2& position, const glm::vec2& size, float texIndex, const glm::vec4& tex_coords)
{
        QuadBufferPtr->position = {position.x, position.y, 0.0f};
        QuadBufferPtr->texCoords = {tex_coords.x, tex_coords.y};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr++;

        QuadBufferPtr->position = {position.x + size.x, position.y, 0.0f};
        QuadBufferPtr->texCoords = {tex_coords.x + tex_coords.z, tex_coords.y};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr++;

        QuadBufferPtr->position = {position.x + size.x, position.y + size.y, 0.0f};
        QuadBufferPtr->texCoords = {tex_coords.x + tex_coords.z, tex_coords.y + tex_coords.w};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr++;

        QuadBufferPtr->position = {position.x, position.y + size.y, 0.0f};
        QuadBufferPtr->texCoords = {tex_coords.x, tex_coords.y + tex_coords.w};
        QuadBufferPtr->texIndex = texIndex;
        QuadBufferPtr++;

        IndexCount += 6;
}

void TextureBatch::SetViewMatrix(glm::mat4 &vm)
{
        viewMatrix = vm;
        shader.Bind();
        glUniformMatrix4fv(shader.GetUniformLocation("ortho"), 1, GL_FALSE, &viewMatrix[0][0]);
}
