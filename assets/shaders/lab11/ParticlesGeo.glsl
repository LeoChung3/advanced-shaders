#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

vec3 getNormal();

// VP is projection*view

layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

float size = 2;
out vec2 uv;


void main()
{
	mat4 VP = u_projection*u_view;

 	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(u_viewPos - pos);
	vec3 up = vec3(0,1,0);
	vec3 right = normalize(cross(toCamera, up));

	vec3 corner0 = pos - right * size - up * size;	//bottom left
	vec3 corner1 = pos + right * size - up * size;	//bottom right
	vec3 corner2 = pos - right * size + up * size;	//top left
	vec3 corner3 = pos + right * size + up * size;	//top right


	uv = vec2(0.0, 0.0);
	gl_Position = VP * vec4(corner0, 1.0);
	EmitVertex();

	uv = vec2(1.0, 0.0);
	gl_Position = VP * vec4(corner1, 1.0);
	EmitVertex();
	uv = vec2(0.0, 1.0);
	gl_Position = VP * vec4(corner2, 1.0);
	EmitVertex();
	uv = vec2(1.0, 1.0);
	gl_Position = VP * vec4(corner3, 1.0);
	EmitVertex();

	EndPrimitive();
}