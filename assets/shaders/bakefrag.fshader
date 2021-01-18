#version 450 core


in vec3 v_Normal;
in float v_color_index;

// layout(location = 0) out vec4 color;
out vec4 color;

void main(){
    color = vec4((v_Normal.x + 1.0) / 2.0,(v_Normal.y + 1.0) / 2.0,(v_Normal.z + 1.0) / 2.0, v_color_index);
}
