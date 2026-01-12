#include "cloth.h"

#include <glm/vec4.hpp>

Cloth::Cloth(glm::vec3 basePos)
{
	for (int loop = 0; loop < m_points.size(); loop++)
	{
		glm::vec3& point = m_points[loop];

		float xDiff = COLUMN_SEPARATION * (loop % COLUMNS);
		float yDiff = LINES_SEPARATION * (loop / COLUMNS);

		point = basePos + glm::vec3(1, 0, 0) * xDiff + glm::vec3(0, 1, 0) * yDiff;
	}
}

void Cloth::Update()
{
	for (int loop = 0; loop < m_points.size(); loop++)
	{
		glm::vec3& point = m_points[loop];
		if (loop < COLUMNS && loop%2 == 0)
		{
			//Point should stay in place
			return;
		}

		//Point should move depending on neighbors
	}
}

const void Cloth::Render(const RenderApi3D& api) const
{
	for (int loop = 0; loop < m_points.size(); loop++)
	{
		const glm::vec3& point = m_points[loop];

		api.solidSphere(point, 0.1f, 4, 4, glm::vec4(1, 0, 0, 1));
	}
}


