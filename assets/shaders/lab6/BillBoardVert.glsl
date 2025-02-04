#version 460 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_model;

void main()
{
	gl_Position = u_model * vec4(a_Pos,1);
}