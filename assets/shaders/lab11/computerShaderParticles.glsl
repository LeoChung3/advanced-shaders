#version 460 core

layout(local_size_x = 64) in;

struct Vertex
{
vec4 position;
vec4 velocity;
};

layout(std430, binding = 0) buffer vertexBuffer
{
Vertex vertices[];
};

void main()
{

	uint idx = gl_GlobalInvocationID.x;

	vertices[idx].position = vec4(0);
	vertices[idx].velocity = vec4(1);
	vertices[idx].velocity.w = 3;

}

