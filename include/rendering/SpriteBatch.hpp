#ifndef Sprite_Batch_hpp
#define Sprite_Batch_hpp

#include <array>
#include "rendering/Batch.hpp"



class SpriteBatch : public Batch
{
public:
void Init();
void SetViewMatrix(glm::mat4 &vm);

void BeginBatch();
void EndBatch();
void Clean();
void DrawQuad(const glm::vec2& position, const glm::vec2& size);
void DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID, const glm::vec4& tex_coords, const glm::uvec4& colors);
//TODO: void DrawRotatedQuad();
void Flush(); //virtual?
private:
void addQuadToBuffer(const glm::vec2& position, const glm::vec2& size, float tex_index, const glm::vec4& tex_coords, const glm::uvec4& colors);
static const size_t MaxTextures = 32;
std::array<uint32_t, MaxTextures> TextureSlots;
uint32_t TextureSlotIndex = 1;

unsigned int texture;
unsigned int texture_n;

struct Vertex
{
        glm::vec3 position;
        //float color[3];
        glm::vec2 texCoords;
        float tex_index;
        glm::uvec4 colors;
        glm::vec3 world_position;
};
Vertex* QuadBuffer = nullptr;
Vertex* QuadBufferPtr = nullptr;
};

#endif
