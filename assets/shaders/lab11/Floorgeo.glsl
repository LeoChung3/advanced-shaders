#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in vec3 tes_fragmentPos[];
in vec3 tes_normal[];
in vec2 tes_texCoord[];
//in vec4 vs_fragmentPosLightSpace;

out vec3 fragmentPos;
out vec3 vertexNormal;
out vec2 texCoord;
//out vec4 fragmentPosLightSpace;

uniform int normalType = 1;

vec3 getNormal();

void main()
{
	for (int i = 0; i < 3; i++)
	{
		//fragmentPosLightSpace = vs_fragmentPosLightSpace;
		texCoord = tes_texCoord[i];
		fragmentPos = tes_fragmentPos[i];
		if(normalType == 0)
		{
			vertexNormal = getNormal();
		}
		if(normalType == 1)
		{
			vertexNormal = tes_normal[i];
		}
		//vertexNormal = vec3(0,1,0);

		gl_Position = gl_in[i].gl_Position;
		EmitVertex();

	}
	EndPrimitive();

}

vec3 getNormal()
{
	vec3 a = tes_fragmentPos[0] - tes_fragmentPos[1];
	vec3 b = tes_fragmentPos[0] - tes_fragmentPos[2];
	return normalize(cross(a,b));
}