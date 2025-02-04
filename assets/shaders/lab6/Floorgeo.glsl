#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vs_fragmentPos[];
in vec2 vs_texCoord[];
in vec3 vs_normal[];
//in vec4 vs_fragmentPosLightSpace;

out vec3 fragmentPos;
out vec2 texCoord;
out vec3 vertexNormal;
//out vec4 fragmentPosLightSpace;

vec3 getNormal();

void main()
{
	for (int i = 0; i < 3; i++)
	{
		//fragmentPosLightSpace = vs_fragmentPosLightSpace;
		texCoord = vs_texCoord[i];
		fragmentPos = vs_fragmentPos[i];
		//vertexNormal = vs_normal[i];
		vertexNormal = getNormal();
		//vertexNormal = vec3(0,1,0);

		gl_Position = gl_in[i].gl_Position;
		EmitVertex();

	}
	EndPrimitive();

}

vec3 getNormal()
{
	vec3 a = vs_fragmentPos[0] - vs_fragmentPos[1];
	vec3 b = vs_fragmentPos[0] - vs_fragmentPos[2];
	return normalize(cross(a,b));
}