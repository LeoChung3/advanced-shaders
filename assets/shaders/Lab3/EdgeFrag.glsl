#version 460 core
layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_originalTexture;
uniform float a;

void main()
{
	float b = a;
	if(b == 1.0)
	{
	
	vec2 inverseScreen = (1.0 / vec2(800, 600));
	vec3 colorCenter = texture(u_originalTexture, texCoords).rgb;
	vec3 colorLeft = texture(u_originalTexture, texCoords + vec2(-1.0, 0.0) * inverseScreen).rgb;
	vec3 colorRight = texture(u_originalTexture, texCoords + vec2(1.0, 0.0) * inverseScreen).rgb;
	vec3 colorUp = texture(u_originalTexture, texCoords + vec2(0.0, 1.0) * inverseScreen).rgb;
	vec3 colorDown = texture(u_originalTexture, texCoords + vec2(0.0, -1.0) * inverseScreen).rgb;

	float lumCenter = dot(colorCenter, vec3(0.2126, 0.7152, 0.0722));
	float lumLeft= dot(colorLeft,	   vec3(0.2126, 0.7152, 0.0722));
	float lumRight = dot(colorRight,   vec3(0.2126, 0.7152, 0.0722));
	float lumUp = dot(colorUp,		   vec3(0.2126, 0.7152, 0.0722));
	float lumDown = dot(colorDown,	   vec3(0.2126, 0.7152, 0.0722));
		
	float edgeHorizontal = abs(lumLeft - lumRight);
	float edgeVertical = abs(lumUp - lumDown);
	float edgeStrength = max(edgeHorizontal, edgeVertical);

	if (edgeStrength > 0.1f)
	{
		ppColour = vec4(1.0);

	}else
	ppColour = vec4(0.0);
	} else
	{
		vec3 rgb = texture(u_originalTexture, texCoords).rgb;
ppColour = vec4(rgb,1);}

}
