#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

out vec3 fragmentPos;
out vec3 vertexNormal;
out vec2 texCoord;

uniform mat4 u_model;


void main()
{

	fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
	vertexNormal = vec3(0.0, 1.0, 0.0);
	texCoord = a_texCoord;
	gl_Position = u_projection * u_view * vec4(fragmentPos, 1.0);
}