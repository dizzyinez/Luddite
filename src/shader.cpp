
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "rendering/Shader.hpp"
#include "rendering/loadShader.hpp"

Shader::Shader()
{
}

bool Shader::Load(const char *vertex_file_path, const char *fragment_file_path)
{
        glID = LoadShaders(vertex_file_path, fragment_file_path);
        return true; //?????
}

unsigned int Shader::GetUniformLocation(const char* uniform)
{
        return glGetUniformLocation(glID, uniform);
}

void Shader::Bind()
{
        glUseProgram(glID);
}
