#version 460 core

layout(location = 0) out vec4 gPosition;




in vec4 fragmentPos;



void main()
{
	    gPosition = vec4(fragmentPos, 1.0);

}


