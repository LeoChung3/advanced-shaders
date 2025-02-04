#version 460 core


out vec4 fragColour;
uniform sampler2D u_texture;
in vec2 uv;
void main()
{
	vec4 sampledCol = texture(u_texture, uv);
	if(sampledCol.a < 1) discard;

	fragColour = sampledCol;
}