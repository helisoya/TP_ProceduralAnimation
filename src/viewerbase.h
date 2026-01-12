#pragma once

#include "camera.h"
#include <glm/vec4.hpp>

struct RenderApi3D;
struct RenderApi2D;
struct GLFWwindow;

struct Viewer {
	char windowName[512];
	GLFWwindow* window;

	Camera camera;

	float pointSize;
	float lineWidth;

	glm::vec4 backgroundColor;

	glm::vec4 lightDir;
	float lightStrength;
	float lightAmbient;
	float specular;
	float specularPow;

	int viewportWidth;
	int viewportHeight;

	void* pCustomShaderData;
	int CustomShaderDataSize;

	void* pSkinningShaderData;
	int SkinningShaderDataSize;

	Viewer(char const* initialWindowName, int initialViewportWidth, int initialViewportHeight);

	int /*exit code*/ run();

	// -----------------------------------
	// override the following functions
	// to create your own viewer
	// -----------------------------------

	virtual void init() = 0;

	virtual void update(double deltaTime) = 0;

	virtual void render3D_Custom(const RenderApi3D& api) const = 0;

	virtual void render3D_Skinning(const RenderApi3D & api) const = 0;

	virtual void render3D(const RenderApi3D& api) const = 0;

	virtual void render2D(const RenderApi2D& api) const = 0;

	virtual void drawGUI() = 0;

};