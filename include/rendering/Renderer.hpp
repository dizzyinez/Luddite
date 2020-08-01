#ifndef Renderer_hpp
#define Renderer_hpp

#include <memory>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "rendering/TextureBatch.hpp"

class Renderer
{
public:
// Renderer();
static void Init();

// void Render3DQuad

static void RenderSprite(const glm::vec2& position, const glm::vec2& size);
static void flushSpriteBatch();
static void updateMatricies(int w, int h);

static void setProjectionWorld();
static void setProjectionScreen();
private:
inline static uint32_t texture;

inline static std::unique_ptr<TextureBatch> spriteBatch;


inline static glm::mat4 worldOrthoMatrix;
inline static glm::mat4 screenOrthoMatrix;
inline static unsigned int orthoMatrixID;

inline static unsigned int vertexarrayid;
inline static Shader basic_shader;
inline static unsigned int quad_position_buffer;
inline static unsigned int quad_element_buffer;
// unsigned int texture;
inline static unsigned int texture_n;

// float QuadPositions[20] =
// {
//         -500.0f, -500.0f, 0.0f,  0.0f,  0.0f,
//         500.0f,  -500.0f, 0.0f,  1.0f,  0.0f,
//         500.0f,   500.0f, 0.0f,  1.0f,  1.0f,
//         -500.0f,  500.0f, 0.0f,  0.0f,  1.0f
// };

static constexpr unsigned int QuadIndices[6] =
{
        0, 1, 2,
        2, 3, 0
};
};

#endif
