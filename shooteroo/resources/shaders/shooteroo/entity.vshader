#version 450 core

layout (location = 0) in vec2 a_Offset;
layout (location = 1) in vec2 a_Position;
layout (location = 2) in vec2 a_texCoords;

out vec2 v_texCoords;

uniform mat4 u_proj;

void main()
{
    // calculate position
	// gl_Position = vec4((u_proj * vec4(a_Offset, 0.f, 1.f)).xy + a_Position, 0.f, 1.f);
	gl_Position = u_proj * vec4(a_Offset + a_Position, 0.f, 1.f);
    v_texCoords = a_texCoords;
}