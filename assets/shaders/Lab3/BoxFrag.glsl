#version 460 core


in vec3 texCoords;

uniform samplerCube u_skybox;

out vec4 colour;

void main()
{
colour = texture(u_skybox, texCoords);
}