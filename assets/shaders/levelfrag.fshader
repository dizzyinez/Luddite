#version 450 core

in vec2 v_TexCoord;
in float v_TexIndex;

out vec4 color;

uniform sampler2D u_Textures[32];

void main(){
  // color = vec4(1,0,0,1);
  int index = int(v_TexIndex);
  color = texture(u_Textures[index], v_TexCoord);
}
