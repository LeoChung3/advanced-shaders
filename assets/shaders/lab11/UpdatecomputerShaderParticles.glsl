#version 460 core

layout(local_size_x = 64) in;

struct Vertex
{
vec4 position;
vec4 velocity;
vec4 startingpos;
vec4 startingvel;
};

layout(std430, binding = 0) buffer vertexBuffer
{
Vertex vertices[];
};

uniform float time;
uniform float u_gravity;
vec3 gravity = vec3(0,-8,0);

void main()
{

	uint idx = gl_GlobalInvocationID.x;

	gravity.y *= u_gravity;

	vertices[idx].position.xyz = vertices[idx].position.xyz + vec3(1,-1,0) * time;
	vertices[idx].velocity.xyz = vertices[idx].velocity.xyz + gravity * time;
	vertices[idx].position.w += time;

}

