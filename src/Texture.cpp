#include "data/Texture.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


Texture::~Texture()
{
  glDeleteTextures(1, &texture_id);
}
