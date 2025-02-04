#version 460 core


struct Vertex
{
vec4 position;
vec4 velocity;
};

layout(std430, binding = 0) buffer vertexBuffer
{
Vertex vertices[];
};

vec3 Ppositions;
vec3 Pvelocity;

void main()
{
	Ppositions = vertices[gl_VertexID].position.xyz;
	Pvelocity = vertices[gl_VertexID].velocity.xyz;

	gl_Position = vec4(Ppositions,1.0);

}