#version 450 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;
layout(location = 3) in uvec4 a_Colors;
layout(location = 4) in vec3 a_WorldPosition;

uniform mat4 ortho;

// // Notice that the "1" here equals the "1" in glVertexAttribPointer
// layout(location = 1) in vec3 vertexColor;

// // Values that stay constant for the whole mesh.
// uniform mat4 MVP;

// //Output data ; will be interpolated for each fragment.
out vec2 v_TexCoord;
out float v_TexIndex;
flat out uvec4 v_Colors;

flat out vec3 v_WorldPosition;

void main(){


      // The color of each vertex will be interpolated
      // to produce the color of each fragment
			v_TexCoord = a_TexCoord;
			v_TexIndex = a_TexIndex;
			v_Colors = a_Colors;
			v_WorldPosition = a_WorldPosition;
			// Output position of the vertex, in clip space : MVP * position
			gl_Position =  ortho * vec4(a_Position, 1.0);
}
