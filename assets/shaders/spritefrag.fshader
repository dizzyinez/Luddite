#version 450 core

in vec2 v_TexCoord;
in float v_TexIndex;
flat in uvec4 v_Colors;

flat in vec3 v_WorldPosition;

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
if (index >= 4)
{
  color = vec4(1,1,1,1);
}
else
{

  int index = int(texture_data.w * 255) - 1;
  vec4 tex_color = vec4(float((v_Colors[index]>>24) & 0xFF)/255, float((v_Colors[index]>>16) & 0xFF)/255, float((v_Colors[index]>>8)  & 0xFF)/255, float((v_Colors[index]) & 0xFF)/255);


  vec3 normal = normalize(texture_data.rgb * 2.0 - 1.0);
  // vec3 normal  = normalize(texture_data.rgb);
  vec3 light_pos = vec3(0.0f,0.0f,10.0f);

  float light_dist = distance(v_WorldPosition, light_pos);
  float light_pow = 100;

  vec3 light_dir = light_pos - v_WorldPosition;
  light_dir = normalize(light_dir);
  float diff = max(dot(normal, light_dir), 0.0f);
  //inverse square law
  float dist_factor = min(50/distance(light_dir, v_WorldPosition), 1);
  diff *= dist_factor;
  // diff = max(diff, min(light_pow/(light_dist+50), 0.6f));
  // diff = min(diff, 1);



vec3 view_dir = normalize(vec3(0.0f, 1.41f, 1.0f));
float spec = 0;

if (dot(normal, light_dir) > 0)
{
  vec3 reflect_dir = reflect(-light_dir, normal);
  spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 3);

}
  //inverse square law
  spec *= dist_factor;

float ambient = 0.5;
// float light = max(spec + diff, ambient);

  // color = vec4(tex_color.rgb * diff, tex_color.a);
  color = vec4(tex_color.rgb * min(ambient + spec + diff, 2.0f), tex_color.a);
  // color = vec4(texture_data.rgb, tex_color.a);
  // color = texture_data;
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
