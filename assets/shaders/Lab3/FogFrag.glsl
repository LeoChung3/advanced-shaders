#version 460 core
layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_originalTexture;
uniform sampler2D u_originalTexture2;

float near = 0.1f;
float far = 1000.f;

float fogFactor;


float LineariseDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	vec3 rgb = texture(u_originalTexture, texCoords).rgb;

	float depthValue = texture(u_originalTexture2, texCoords).r;
	float linearDepth = LineariseDepth(depthValue);

	vec3 fogColour = vec3(1,1,1);

	//float depthNormalised = (linearDepth - near) / (far - near);

	float fogDensity = 0.05;
	fogFactor = 1.0 - exp(-fogDensity * linearDepth);

	//mix colour, fog colour, depth
	vec3 fog = mix(rgb,fogColour, fogFactor);

	ppColour = vec4(fog, 1);
}
