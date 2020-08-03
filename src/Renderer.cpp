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
        unsigned char *data = stbi_load("../assets/textures/place_holder_normals.png", &width, &height, &nrChannels, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);


        spriteBatch = std::make_unique<TextureBatch>();
        spriteBatch->Init();
        spriteBatch->BeginBatch();

        // updateMatricies(1024, 768);
}
void Renderer::RenderSprite(const glm::vec2& position, const glm::vec2& size)
{
        CheckGLError();
        spriteBatch->DrawQuad(position, size, texture);
}

void Renderer::flushSpriteBatch()
{
        spriteBatch->EndBatch();
        spriteBatch->Flush();
        spriteBatch->BeginBatch();
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
        spriteBatch->SetViewMatrix(vp);
        glViewport(0, 0, width, height);

        projection = glm::ortho(0.0f, width, height, 0.0f); //reminder: this function only likes floats and seems to fail with integers
        vp = projection * view;
        screenOrthoMatrix = vp;
}

void Renderer::setProjectionWorld()
{
        spriteBatch->SetViewMatrix(worldOrthoMatrix);
}

void Renderer::setProjectionScreen()
{
        spriteBatch->SetViewMatrix(screenOrthoMatrix);
}
