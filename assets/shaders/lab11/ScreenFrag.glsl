#version 460 core
layout(location = 0) out vec4 ppColour;

in vec2 texCoords;

uniform sampler2D u_originalTexture;
uniform float Gamma;
// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
//https://github.com/dmnsgn/glsl-tone-map/blob/main/aces.glsl
vec3 aces(vec3 x) {
	const float a = 2.51;
	const float b = 0.03;
	const float c = 2.43;
	const float d = 0.59;
	const float e = 0.14;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
	vec3 rgb = texture(u_originalTexture, texCoords).rgb;
	rgb = aces(rgb);
	rgb = pow(rgb, vec3(1.0 / Gamma)); // gamma correction
	rgb = clamp(rgb, 0.0, 1.0);


ppColour = vec4(rgb,1);
}
