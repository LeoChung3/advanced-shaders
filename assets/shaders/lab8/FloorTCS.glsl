#version 460 core

layout(vertices = 3) out;


layout (std140, binding = 0) uniform b_camera
{
	uniform mat4 u_view;
	uniform mat4 u_projection;
	uniform vec3 u_viewPos;
};

in vec3 vs_fragmentPos[];
//in vec3 vs_normal;
in vec2 vs_texCoord[];

out vec3 tcs_fragmentPos[];
//out vec3 tcs_normal;
out vec2 tcs_texCoord[];

uniform float tesslevel = 10.0f;

float GetTessLevel(float Distance0, float Distance1)
{
	
	float AvgDistance = (Distance0 + Distance1) / 2.0;
	//float a = 3;
	//float b = 1;
	//float tesslevel2 = ( a * AvgDistance) + b;

	//float num = step(tesslevel, AvgDistance);
	//return tesslevel2;
	//calculate tesselation levels
	if (AvgDistance <= 10)
	{
		return tesslevel;
	}
	else if (AvgDistance <= 20)
	{
		return tesslevel - 3;
	}
	else if (AvgDistance <= 300)
	{
		return tesslevel - 5;
	}
	else
	{
		return 14;
	}
}
void main()
{
	float eyeToVertexDist0 = distance(u_viewPos, vs_fragmentPos[0]);
	float eyeToVertexDist1 = distance(u_viewPos, vs_fragmentPos[1]);
	float eyeToVertexDist2 = distance(u_viewPos, vs_fragmentPos[2]);


	/*gl_TessLevelOuter[0] = tesslevel;
	gl_TessLevelOuter[1] = tesslevel;
	gl_TessLevelOuter[2] = tesslevel;
	gl_TessLevelInner[0] = tesslevel;*/
	if(gl_InvocationID == 0)
	{
	gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDist1, eyeToVertexDist2);
	gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDist2, eyeToVertexDist0);
	gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDist0, eyeToVertexDist1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
	}

	tcs_fragmentPos[gl_InvocationID] = vs_fragmentPos[gl_InvocationID];
	//tcs_normal[gl_InvocationID] = vs_normal[gl_InvocationID];
	tcs_texCoord[gl_InvocationID] = vs_texCoord[gl_InvocationID];
}