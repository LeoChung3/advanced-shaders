#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba8) uniform image2D outputImg;


//uniform sampler2D u_image;
uniform sampler2D heightmap;
//lab 9
//original 512, 512
//snowdon 400 400
vec2 u_imageSize = vec2(512,512);

uniform int u_octaves = 1;
uniform float u_frequency = 10.f;
uniform float u_amplitude = 1.5f;
uniform float u_persistence = 0.5f;
uniform float u_lacunartiy = 2.0f;

int octaves = 1;
float frequency;
float amplitude;
float persistence;
float lacunartiy;


float remap(float currValue, float  inMin,float inMax,float outMin, float outMax) {
	
	float t = (currValue - inMin) / (inMax - inMin);
	return mix(outMin, outMax, t);
}


// The MIT License
// Copyright © 2013 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// https://www.youtube.com/c/InigoQuilez
// https://iquilezles.org/
//
// https://www.shadertoy.com/view/Xsl3Dl



vec2 hash2(vec2 p) 
{
	p = vec2(dot(p, vec2(127.1, 311.7)),
		dot(p, vec2(269.5, 183.3)));

	return -1.0 + 2.0 * fract(sin(p) * 43758.5453123 );
}

float noise(in vec2 p)
{
	vec2 i = floor(p);
	vec2 f = fract(p);

	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(dot(hash2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
		dot(hash2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
		mix(dot(hash2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
			dot(hash2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

float FBM(vec2 position, int oct)
{
	float total = 0.0f;
	float totalAmplitude = 0.0f;
	for(int i = 0; i < oct; i++)
	{
	float p;

	p = noise(position * frequency) * amplitude;
	total += p;
	frequency *= u_lacunartiy;
	amplitude *= persistence;
	totalAmplitude += amplitude;
	}
	
	
	return (u_amplitude*total / totalAmplitude);
}
void main()
{
	int a = u_octaves;
	frequency = u_frequency;
	amplitude = u_amplitude;
	persistence = u_persistence;
	lacunartiy = u_lacunartiy;

	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 uv = vec2(pixel_coords) / vec2(u_imageSize);

	float p;

	//p = noise(uv * frequency) * amplitude;

	p = FBM(uv, a);
	p = remap(p, -1,1,0,1);


	vec3 pixelCol = (vec3(p));


	imageStore(outputImg, pixel_coords, vec4(pixelCol,1));

}

