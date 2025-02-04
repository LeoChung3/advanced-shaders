#version 460 core
layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_originalTexture;
float near = 0.1f;
float far = 100.f;

float LineariseDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{


	float depthValue = texture(u_originalTexture, texCoords).r;
	float linearDepth = LineariseDepth(depthValue);
	float depthNormalised = linearDepth / far;
	ppColour = vec4(vec3(depthNormalised), 1);

}
