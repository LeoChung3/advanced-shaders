#version 460 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;


layout(std140, binding = 3) uniform b_camera2D
{
	uniform mat4 u_view2D;
	uniform mat4 u_projection2D;
};

out vec2 texCoord;

uniform mat4 u_model;

void main()
{
	vec3 fragmentPos = vec3(u_model * vec4(a_vertexPosition, 1.0));
	texCoord = a_texCoord;
	gl_Position = u_projection2D * u_view2D * vec4(fragmentPos,1.0);
}