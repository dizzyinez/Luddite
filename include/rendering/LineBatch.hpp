#pragma once

#include "rendering/Shader.hpp"
#include <glm/glm.hpp>

struct LineBatch
{
        void Init();
        void Flush();

        void DrawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        void SetViewMatrix(glm::mat4 vm)
        {
                viewMatrix = vm;
        }

        unsigned int VAO = 0;
        unsigned int VBO = 0;

        Shader shader;

        glm::mat4 viewMatrix;
};