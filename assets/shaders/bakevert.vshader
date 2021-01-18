#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in float a_color_index;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 normalMatrix;

smooth out vec3 v_Normal;
out float v_color_index;

void main()
{
    // mat4 modle = model;
    mat3 normal_matrix = mat3(normalMatrix);
    // mat4 modle = mat4(1.0f);
    // mat3 normalMatrix = mat3(1.0f);

    v_Normal = normalize(normal_matrix * a_Normal);

    // vec3 normal = normalize(normalMatrix * a_Normal);
    // v_Normal = vec3(normal.x, -normal.z, -normal.y);

    v_color_index = a_color_index;
    gl_Position = mvp * model * vec4(a_Position, 1.0);
    // gl_Position = vec4(a_Position, 1.0f);
}