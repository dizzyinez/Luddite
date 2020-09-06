#ifndef Renderer_hpp
#define Renderer_hpp

#include <memory>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "rendering/TextureBatch.hpp"
#include "rendering/SpriteBatch.hpp"

class Renderer
{
public:
// Renderer();
static void Init();

// void Render3DQuad

static void RenderSquare(const glm::vec2& position, const glm::vec2& size); //TODO: add color
static void RenderTexture(const glm::vec2& position, const glm::vec2& size, uint32_t texture_id, const glm::vec4& tex_coords); //TODO: add color
static void RenderSprite(const glm::vec2& position, const glm::vec2& size, uint32_t texture_id, const glm::vec4& tex_coords, const glm::uvec4& colors);
static void flushTextureBatch();
static void flushSpriteBatch();
static void updateMatricies(int w, int h);

static void setProjectionWorld();
static void setProjectionScreen();
private:
inline static uint32_t texture;

inline static std::unique_ptr<TextureBatch> texture_batch;
inline static std::unique_ptr<SpriteBatch> sprite_batch;


inline static glm::mat4 worldOrthoMatrix;
inline static glm::mat4 screenOrthoMatrix;
inline static unsigned int orthoMatrixID;

inline static unsigned int vertexarrayid;
// inline static Shader basic_shader;
inline static unsigned int quad_position_buffer;
inline static unsigned int quad_element_buffer;
// unsigned int texture;
inline static unsigned int texture_n;

static constexpr unsigned int QuadIndices[6] =
{
        0, 1, 2,
        2, 3, 0
};
};

#endif
