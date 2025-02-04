#version 460 core

layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba8) uniform image2D outputImg;


uniform sampler2D u_image;
uniform sampler2D heightmap;
//lab 9
//original 512, 512
//snowdon 400 400
vec2 u_imageSize = vec2(512,512);
uniform float scale = 1;
void main()
{
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 uv = vec2(pixel_coords) / vec2(u_imageSize);

	//purple colour
	//vec4 pixelCol = vec4(1.0,0.5,1.0,1.0);

	//image to gui
	vec4 pixelCol = texture(u_image, uv);

	//inverse of image
	//vec3 a = texture(u_image, uv).rgb;
	//a = 1 - a;
	//vec4 pixelCol = vec4(a,1.0);

	//blur of image
	//vec4 colourSum = vec4(0.0);
	//int sampleCount = 0;
	//int radius = 10;
	//for(int x = -radius; x <= radius; ++x)
	//{
	//	for(int y = -radius; y <= radius; ++y)
	//	{
	//		vec2 samplePos = uv + vec2(x,y) / u_imageSize;
	//		samplePos = clamp(samplePos, vec2(0.0), vec2(1.0));
	//
	//		colourSum += texture(u_image, samplePos);
	//		sampleCount++;
	//	}
	//}
	//vec4 pixelCol = colourSum / float(sampleCount);

	//CDM computing for height map

	float right = (textureOffset(u_image, uv, ivec2(1,0)).r) * scale;
	float left  = (textureOffset(u_image, uv, ivec2(-1,0)).r) * scale;
	float down  = (textureOffset(u_image, uv, ivec2(0,1)).r) * scale;
	float up    = (textureOffset(u_image, uv, ivec2(0,-1)).r) * scale;

	float lr = left-right;
	float du = up - down;
	vec3 tes_CMnormal = normalize(vec3(lr, 2.0, du));

	pixelCol = vec4(tes_CMnormal, 1.0);
	imageStore(outputImg, pixel_coords, pixelCol);

}

