#ifndef Texture_Batch_hpp
#define Texture_Batch_hpp

#include <array>
#include "rendering/Batch.hpp"


class TextureBatch : public Batch
{
public:
        void Init();
        void SetViewMatrix(glm::mat4 &vm);

        void BeginBatch();
        void EndBatch();
        void Clean();
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID, const glm::vec4& tex_coords, const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
//TODO: void DrawRotatedQuad();
        void Flush(); //virtual?
private:
        void addQuadToBuffer(const glm::vec2& position, const glm::vec2& size, float texIndex, const glm::vec4& tex_coords, const glm::vec4& color);
        static const size_t MaxTextures = 16;
        std::array<uint32_t, MaxTextures> TextureSlots;
        uint32_t TextureSlotIndex = 1;

        unsigned int WhiteTexture = 0;
        uint32_t WhiteTextureSlot = 0;

        unsigned int texture;
        unsigned int texture_n;

        struct Vertex
        {
                glm::vec3 position;
                glm::vec4 color;
                glm::vec2 texCoords;
                float texIndex;
        };
        Vertex* QuadBuffer = nullptr;
        Vertex* QuadBufferPtr = nullptr;
};

#endif
