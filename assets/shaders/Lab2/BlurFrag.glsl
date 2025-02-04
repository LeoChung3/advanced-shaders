#version 460 core
layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_originalTexture;
float blurRadius = 5.0f;

void main()
{
	vec2 texelSize = 1.0 / vec2(800, 600);

	vec3 result = vec3(0.0);
	float samplesTaken = 0.0;

	for(int x = -1; x <=1;++x)
	{
		for (int y = -1; y <=1; ++y)
		{
		vec2 offset = vec2(float(x), float(y)) * texelSize * blurRadius;
		vec3 sampleCol = texture(u_originalTexture, texCoords + offset).rgb;
		result += sampleCol;
		samplesTaken++;
		}
	}

	result /= samplesTaken;


ppColour = vec4(result,1.0);
}
