#version 450 core

layout (location = 0) out vec4 o_Color;

in vec2 v_texCoords;

uniform sampler2D u_texture;

void main()
{
	o_Color = texture(u_texture, v_texCoords);
}