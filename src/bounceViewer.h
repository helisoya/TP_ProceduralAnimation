#pragma once

#include "viewerbase.h"
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define MaxBounces 10

struct VertexShaderAdditionalDataBounce {
	glm::vec4 Pos;
	glm::vec4 bouncesData[MaxBounces];
	glm::vec4 bouncesPosition[MaxBounces];
	/// beware of alignement (std430 rule)
};


struct BounceViewer : Viewer {

	int currentBounceIndex;
	float decaySpeed;
	float strength;
	float distanceFactor;

	//--
	glm::vec2 mousePos;
	//--	
	bool leftMouseButtonPressed;
	bool altKeyPressed;
	//--
	VertexShaderAdditionalDataBounce additionalShaderData;

	BounceViewer();

	void init() override;
	void update(double elapsedTime) override;
	void render3D_Custom(const RenderApi3D& api) const override;
	void render3D_Skinning(const RenderApi3D& api) const override;

	void render3D(const RenderApi3D& api) const override;
	void render2D(const RenderApi2D& api) const override;
	void drawGUI() override;
};

