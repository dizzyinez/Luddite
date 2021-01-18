#ifndef Shader_hpp
#define Shader_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "loadShader.hpp"
class Shader
{
public:
        Shader();
        bool Load(const char *vertex_file_path, const char *fragment_file_path);
        unsigned int GetUniformLocation(const char* uniform);
        void Bind();
        bool Loaded() {return glID != 0;}
private:
        unsigned int glID = 0;
};

#endif
