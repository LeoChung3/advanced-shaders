#version 460 core

layout(triangles, equal_spacing, ccw) in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec2 v0, vec2 v1, vec2 v2);


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;

};

uniform sampler2D u_heightMap;

in vec3 tcs_fragmentPos[];
//in vec3 tcs_normal[];
in vec2 tcs_texCoord[];

out vec3 tes_fragmentPos;
out vec2 tes_texCoord;
uniform float scale = 10;
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;

}
void main()
{
	tes_texCoord = interpolate2D(tcs_texCoord[0],tcs_texCoord[1],tcs_texCoord[2]);


	float height = texture(u_heightMap, tes_texCoord).r;

	tes_fragmentPos = interpolate3D(tcs_fragmentPos[0], tcs_fragmentPos[1], tcs_fragmentPos[2]);
	tes_fragmentPos.y += height*scale;

	gl_Position = u_projection * u_view * vec4(tes_fragmentPos, 1.0);
}