#version 430 core

// You can compil and refresh the shader at runtime using the F7 key

#define BufferAttribVertex 0
#define BufferAttribNormal 1
#define BufferAttribColor 2
#define BufferAttribBoneIndices 3
#define BufferAttribBoneWeights 4


//-- Uniform are variable that are common to all vertices of the drawcall
uniform mat4 Model; // Model matrix
uniform mat4 View;  // View matrix
uniform mat4 Projection; // Projection Matrix
uniform float Time; // Elapsed time since the begining of the program

//-- attributes can change for each vertex
layout(location = BufferAttribVertex) in vec3 Position; // Position of current vertex
layout(location = BufferAttribNormal) in vec3 Normal;		// Normal of current vertex
layout(location = BufferAttribColor) in vec4 Color;			// Color of current vertex
layout(location = BufferAttribBoneIndices) in ivec4 BoneIndices;			// Indices of skinning bones
layout(location = BufferAttribBoneWeights) in vec4 BoneWeights;			// Weights of skinning bones

//-- Here is the GPU counterpart of the SkinningShaderAdditionalData structure
layout(std430, binding= 3) buffer bufferData
{ 
	float Something;
} SkinningData;

out block //define the additional output that will be received by the fragment shader
{
	vec4 Color;
	vec3 CameraSpacePosition;
	vec3 CameraSpaceNormal;
} Out;

void main()
{
	mat4 MV = View * Model;
	
	vec4 NewPos = vec4(Position, 1.f);
	NewPos.y += BoneWeights.x;
	Out.CameraSpacePosition = vec3(MV * NewPos);
	Out.CameraSpaceNormal = vec3(MV * vec4(Normal, 0.0f));
	Out.Color.r *= SkinningData.Something;
	Out.Color.g = float(BoneIndices.x) * 0.333;
	Out.Color.b = 0.;

	Out.Color.r = (sin(Time) + 1.0f)*0.5f;
	gl_Position = Projection * MV * NewPos;
}