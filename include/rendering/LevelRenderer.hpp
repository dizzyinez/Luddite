#pragma once
#include "level/LevelGenerator.hpp"
#include "rendering/Shader.hpp"
#include <array>
#include <vector>
#include <stdint.h>

struct LevelRenderer
{
        void Init();
        void SetLevel(const Level& level);
        void Render();
        void SetViewMatrix(glm::mat4 vm);
        glm::mat4 viewMatrix;
        Shader shader;
        static constexpr size_t MaxTextures = 16;
        std::array<uint32_t, MaxTextures> TextureSlots;
        static constexpr uint32_t TextureSlotIndex = 4;
        uint32_t WhiteTexture;
        uint32_t RedTexture;
        uint32_t BlueTexture;
        uint32_t GreenTexture;
        static constexpr uint32_t WhiteTextureSlot = 0;
        unsigned int texture;
        // unsigned int texture_n;
        uint32_t VBO;
        uint32_t VAO;
        uint32_t IBO;

        struct Vertex
        {
                glm::vec2 position;
                glm::vec2 texCoords;
                float texIndex;
        };
        std::vector<Vertex> VA;
        std::vector<uint32_t> IA;
};