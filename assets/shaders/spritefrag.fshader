#version 450 core

in vec2 v_TexCoord;
in float v_TexIndex;
flat in uvec4 v_Colors;

out vec4 o_Color;

uniform sampler2D u_Textures[32];

void main(){
  // color = vec4(1,0,0,1);
  vec4 texture_data = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
  // vec4 tex_color = texture(u_Textures[0], vec2(texture_data.w, 0.0f));
  vec4 color;

// color = vec4(float((v_Colors.x) & 0xFF)/255, float((v_Colors.x>>8) & 0xFF)/255, float((v_Colors.x>>16)  & 0xFF)/255, float((v_Colors.x>>24) & 0xFF)/255);
// color = vec4(float(0xFFFFFFFF & 0xFF), 1,1,1);

if (texture_data.w == 0)
{
  color = vec4(0,0,0,0);
}
else
{

  int index = int(texture_data.w * 255) - 1;
  vec4 tex_color = vec4(float((v_Colors[index]) & 0xFF)/255, float((v_Colors[index]>>8) & 0xFF)/255, float((v_Colors[index]>>16)  & 0xFF)/255, float((v_Colors[index]>>24) & 0xFF)/255);


  vec3 normal = texture_data.rgb;
  normal = normalize(normal * 2.0 - 1.0);
  float dotProduct = dot(normal, normalize(vec3(-1.0f,0.0f, -1.0f)));
  if (dotProduct < 0.3)
  {
    color = tex_color * 0.3;
  }
  else
  {
    color = tex_color * dotProduct;
  }
}

  o_Color = color;
  // o_Color = vec4(color, tex_color.a); // * v_color;

    // float dotProduct = dot(vec3(texture_data.r, texture_data.g, texture_data.b), normalize(vec3(1.0f, 0.0f, 0.0f)));
    // else
    // {
    //   color = tex_color;
    //   // color = vec4(dotProduct, 0, 0, 1);
    // }
    // if (dotProduct > 0.5)
    // {
    //   color = tex_color + vec4(0.1f,0.1f, 1.0f, 0.0f) * max(dotProduct, 0) * 5;
    // }
    // else
    // {
    //   color = tex_color;
    // }
  // color = vec4(1.0f,1.0f, 1.0f, 1.0f);
}
