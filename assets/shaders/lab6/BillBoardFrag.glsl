#version 460 core

layout(location = 0) out vec4 fragcolour;


in vec2 uv;



uniform sampler2D tree;


void main()
{
	vec4 sampledCol = texture(tree, uv);
	//float alphaMask = step(0,sampledCol.a);
	if (sampledCol.a == 0.0) discard;

	fragcolour = sampledCol;

}