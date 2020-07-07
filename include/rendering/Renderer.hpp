#ifndef Renderer_hpp
#define Renderer_hpp

#include <glm/glm.hpp>
#include "Shader.hpp"
#include "rendering/TextureBatch.hpp"

class Renderer
{
public:
Renderer();
void Init();

// void Render3DQuad

void RenderSprite(const glm::vec2& position, const glm::vec2& size);
void flushSpriteBatch();
void updateMatricies(int w, int h);

void setProjectionWorld();
void setProjectionScreen();
private:
uint32_t texture;

TextureBatch* spriteBatch;


glm::mat4 worldOrthoMatrix;
glm::mat4 screenOrthoMatrix;
unsigned int orthoMatrixID;

unsigned int vertexarrayid;
Shader basic_shader;
unsigned int quad_position_buffer;
unsigned int quad_element_buffer;
// unsigned int texture;
unsigned int texture_n;

float QuadPositions[20] =
{
        -500.0f, -500.0f, 0.0f,  0.0f,  0.0f,
        500.0f,  -500.0f, 0.0f,  1.0f,  0.0f,
        500.0f,   500.0f, 0.0f,  1.0f,  1.0f,
        -500.0f,  500.0f, 0.0f,  0.0f,  1.0f
};

unsigned int QuadIndices[6] =
{
        0, 1, 2,
        2, 3, 0
};
};

#endif
