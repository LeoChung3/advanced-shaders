#version 460 core
layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_originalTexture;
uniform sampler2D blurTexture;
uniform sampler2D DepthTexture;

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
	vec3 blury = texture(blurTexture, texCoords).rgb;

	float depthValue = texture(DepthTexture, texCoords).r;
	float linearDepth = LineariseDepth(depthValue);

	float fogDensity = 0.05;
	fogFactor = 1.0 - exp(-fogDensity * linearDepth);

	vec3 blured = mix(rgb,blury, fogFactor);


	ppColour = vec4(blured, 1);

}
