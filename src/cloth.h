#pragma once

#include <array>
#include <glm/vec3.hpp>

#include "renderapi.h"


class Cloth
{
private:
	constexpr static int COLUMNS = 10;
	constexpr static int LINES = 10;
	constexpr static float COLUMN_SEPARATION = 0.5f;
	constexpr static float LINES_SEPARATION = 0.5f;

	std::array<glm::vec3, COLUMNS* LINES> m_points;

public:
	Cloth(glm::vec3 basePos);
	void Update();
	const void Render(const RenderApi3D& api) const;
};