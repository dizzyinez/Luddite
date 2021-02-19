#version 450 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;

uniform mat4 ortho;

out vec2 v_TexCoord;
out float v_TexIndex;

void main(){


      // The color of each vertex will be interpolated
      // to produce the color of each fragment
			v_TexCoord = a_TexCoord;
			v_TexIndex = a_TexIndex;

			// Output position of the vertex, in clip space : MVP * position
			gl_Position = ortho * vec4(a_Position, 0.0, 1.0);

}
