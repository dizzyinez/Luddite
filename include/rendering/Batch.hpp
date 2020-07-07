#ifndef Batch_hpp
#define Batch_hpp
#include "rendering/Shader.hpp"
#include <glm/glm.hpp>

class Batch
{
public:
virtual void Init() = 0;

virtual void BeginBatch() = 0;
virtual void EndBatch() = 0;
virtual void Clean() = 0;

virtual void Flush() = 0; //virtual?
virtual void DrawQuad(const glm::vec2& position, const glm::vec2& size) = 0;
//TODO: void DrawRotatedQuad();

virtual void SetViewMatrix(glm::mat4 vm)
{
        viewMatrix = vm;
}

protected:
size_t MaxQuadCount;// = 5000;
size_t MaxVertexCount;// = MaxQuadCount * 4;
size_t MaxIndexCount;// = MaxQuadCount * 6;

unsigned int QuadVA = 0;
unsigned int QuadVB = 0;
unsigned int QuadIB = 0;

uint32_t IndexCount = 0;

Shader shader;

glm::mat4 viewMatrix;
};

#endif
