#version 330 core

in vec2 v_TexCoord;
in float v_TexIndex;

out vec4 color;

uniform sampler2D u_Textures[32];

void main(){
  // color = vec4(1,0,0,1);
  int index = int(v_TexIndex);
  color = texture(u_Textures[1], v_TexCoord); // * v_color;
  //   // Output color = color specified in the vertex shader,
  //   // interpolated between all 3 surrounding vertices
  //   vec4 tex_color = texture(image1, Texture_coordinate);
  //   if (tex_color.a < 0.1)
  //     discard;
  //   vec4 tex_n_color = texture(image2, Texture_coordinate);
  //   if (tex_n_color.a < 0.1)
  //       discard;
  //   float dotProduct = dot(tex_n_color, normalize(vec4(1.0f,0.0f, 0.0f, 0.0f)));
  //   // if (dotProduct < 0.3)
  //   // {
  //   //   color = tex_color * 0.6;
  //   // }
  //   // else
  //   // {
  //   //   color = tex_color;
  //   //   // color = vec4(dotProduct, 0, 0, 1);
  //   // }
  //   if (dotProduct > 0.5)
  //   {
  //     color = tex_color + vec4(0.1f,0.1f, 1.0f, 0.0f) * max(dotProduct, 0) * 5;
  //   }
  //   else
  //   {
  //     color = tex_color;
  //   }
  // // color = vec4(1.0f,1.0f, 1.0f, 1.0f);
}
