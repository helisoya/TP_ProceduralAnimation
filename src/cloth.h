#pragma once

#include <array>
#include <glm/vec3.hpp>

#include "cloth_point.h"
#include "renderapi.h"
#include "viewerbase.h"


struct Cloth : Viewer
{
private:
	constexpr static int COLUMNS = 10;
	constexpr static int LINES = 10;
	constexpr static float COLUMN_SEPARATION = 0.5f;
	constexpr static float LINES_SEPARATION = 0.5f;

	glm::vec2 mousePos;
	glm::vec3 mousePosWorld;
	//--	
	bool leftMouseButtonPressed;
	bool altKeyPressed;

	std::array<ClothPoint, COLUMNS* LINES> m_points;
	glm::vec3 basePos;

	ClothPoint* closestPoint = nullptr;

	float baseDistance = 0.2f;
	float stifness = 5.0f;
	float gravity = 1.0f;

public:
	Cloth();
	void init() override;
	void update(double elapsedTime) override;
	void render3D_Custom(const RenderApi3D& api) const override;
	void render3D_Skinning(const RenderApi3D& api) const override;

	void render3D(const RenderApi3D& api) const override;
	void render2D(const RenderApi2D& api) const override;
	void drawGUI() override;
};