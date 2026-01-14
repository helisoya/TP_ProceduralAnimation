#version 430 core

// You can compil and refresh the shader at runtime using the F7 key

#define BufferAttribVertex 0
#define BufferAttribNormal 1
#define BufferAttribColor 2

#define MaxBounces 10


//-- Uniform are variable that are common to all vertices of the drawcall
uniform mat4 Model; // Model matrix
uniform mat4 View;  // View matrix
uniform mat4 Projection; // Projection Matrix
uniform float Time; // Elapsed time since the begining of the program

//-- attributes can change for each vertex
layout(location = BufferAttribVertex) in vec3 Position; // Position of current vertex
layout(location = BufferAttribNormal) in vec3 Normal;		// Normal of current vertex
layout(location = BufferAttribColor) in vec4 Color;			// Color of current vertex

//-- Here is the GPU counterpart of the VertexShaderAdditionalData structure
layout(std430, binding= 3) buffer bufferData
{ 
	vec4 center;
	vec4 bouncesData[MaxBounces];
	vec4 bouncesPosition[MaxBounces];
} Data;

out block //define the additional output that will be received by the fragment shader
{
	vec4 Color;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} Out;

void main()
{
	mat4 MV = View * Model;
	vec4 NewPos = vec4(Position, 1);
	
	//float XParity = mod(3.*Position.x + Time, 2.0f);
	//XParity = step(XParity, 0.2f);

	//float distToBounce = distance(NewPos,Data.bouncePosition);
	//if(distToBounce <= Data.bounceData.x){
	//	XParity = distToBounce * Data.bounceData.x;
	//	XParity *= sin(Time * 15.0f) * Data.bounceData.y;
	//}

	float XParity = 0.0f;
	float distToBounce = 0.0f;
	int i = 0;

	for(i = 0;i < MaxBounces;i++){
		distToBounce = distance(NewPos,Data.bouncesPosition[i]);
		XParity += ( Data.bouncesData[i].x / distToBounce*Data.bouncesData[i].z) 
			* sin(Time * 15.0f) * Data.bouncesData[i].y;
	}
	
	NewPos.x += Data.center.x;
	NewPos.y += XParity * 0.25 + Data.center.y;
	NewPos.z += Data.center.z;
	
	Out.CameraSpacePosition = vec3(MV * NewPos);
	Out.CameraSpaceNormal = vec3(MV * vec4(Normal, 0.0f));
	Out.Color = Color;
	Out.Color.r = (sin(NewPos.x) + 1.0f)*0.5f;
	//gl_position is always an output and is the resulting vertex pos that will be feeded to fragment shader
	gl_Position = Projection * MV * NewPos;
}