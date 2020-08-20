#include "rendering/Renderer.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "CheckGLError.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "rendering/loadShader.hpp"

void Renderer::Init()
{

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        stbi_set_flip_vertically_on_load(false);

        int width, height, nrChannels;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        unsigned char *data = stbi_load("../assets/textures/untitled.png", &width, &height, &nrChannels, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        texture_batch = std::make_unique<TextureBatch>();
        texture_batch->Init();
        texture_batch->BeginBatch();

        sprite_batch = std::make_unique<SpriteBatch>();
        sprite_batch->Init();
        sprite_batch->BeginBatch();

        // updateMatricies(1024, 768);
}
void Renderer::RenderTexture(const glm::vec2& position, const glm::vec2& size)
{
        CheckGLError();
        texture_batch->DrawQuad(position, size, texture);
}

void Renderer::RenderSprite(const glm::vec2& position, const glm::vec2& size)
{
        CheckGLError();
        sprite_batch->DrawQuad(position, size, texture, glm::uvec4(0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFFFF00FF));
}

void Renderer::flushTextureBatch()
{
        texture_batch->EndBatch();
        texture_batch->Flush();
        texture_batch->BeginBatch();
}

void Renderer::flushSpriteBatch()
{
        sprite_batch->EndBatch();
        sprite_batch->Flush();
        sprite_batch->BeginBatch();
}
void Renderer::updateMatricies(int w, int h)
{
//TODO: make a maximum aspect ratio so ultra-massive-wide-screen (or just a big vertical shrink) won't make insane viewing distances possible
        float width = float(w);
        float height = float(h);
        float ar = width / height;
        float half_width = (1000 * ar) / 2;
        glm::mat4 view = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0));

        glm::mat4 projection = glm::ortho(-half_width, half_width, 500.0f, -500.0f);
        glm::mat4 vp = projection * view;
        worldOrthoMatrix = vp;
        sprite_batch->SetViewMatrix(vp);
        glViewport(0, 0, width, height);

        projection = glm::ortho(0.0f, width, height, 0.0f); //reminder: this function only likes floats and seems to fail with integers
        vp = projection * view;
        screenOrthoMatrix = vp;
}

void Renderer::setProjectionWorld()
{
        sprite_batch->SetViewMatrix(worldOrthoMatrix);
        texture_batch->SetViewMatrix(worldOrthoMatrix);
}

void Renderer::setProjectionScreen()
{
        sprite_batch->SetViewMatrix(screenOrthoMatrix);
        texture_batch->SetViewMatrix(screenOrthoMatrix);
}
