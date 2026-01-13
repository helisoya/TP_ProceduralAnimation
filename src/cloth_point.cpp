#include "cloth_point.h"
#include <iostream>
#include <vector>
#include <glm/ext/matrix_transform.hpp>


void ClothPoint::CalculateNeighborForce(ClothPoint* neighbor, const float gravity, const float baseDistance, const float stifness, const double deltaTime)
{
	float distance = glm::distance(neighbor->point, point);
	if (distance == 0)
		return;

	glm::vec3 calculatedForce = glm::normalize(neighbor->point - point) * ((distance - baseDistance) * stifness / 2);
	force += calculatedForce * (float)deltaTime;
	neighbor->force -= calculatedForce * (float)deltaTime;

}

void ClothPoint::Init(ClothPoint* leftNeighbor, ClothPoint* downNeighbor)
{
	m_leftNeighbor = leftNeighbor;
	m_downNeighbor = downNeighbor;
}

void ClothPoint::CalculateForces(const float gravity, const float baseDistance, const float stifness, const double deltaTime)
{
	force = gravity * glm::vec3(0, -1, 0) * (float)deltaTime;

	if (m_leftNeighbor != nullptr)
	{
		CalculateNeighborForce(m_leftNeighbor, gravity, baseDistance, stifness, deltaTime);
	}
	if (m_downNeighbor != nullptr)
	{
		CalculateNeighborForce(m_downNeighbor, gravity, baseDistance, stifness, deltaTime);
	}
}

void ClothPoint::ApplyForces()
{
	//std::cout << force.x << " " << force.y << " " << force.z << "\n";
	if (!shouldMove)
		return;
	point += force;
}

void ClothPoint::Render(const RenderApi3D& api) const
{
	api.solidSphere(point, 0.1f, 4, 4, glm::vec4(1, 0, 0, 1));

	std::vector<glm::vec3> vertices = {};

	bool hasLeftNeighbor = m_leftNeighbor != nullptr;
	bool hasUpNeighbor = m_downNeighbor != nullptr;

	if (!hasLeftNeighbor && !hasUpNeighbor)
		return;
	
	if (hasLeftNeighbor)
	{
		vertices.push_back(m_leftNeighbor->point);
		vertices.push_back(point);
	}
	if (hasUpNeighbor)
	{
		vertices.push_back(point);
		vertices.push_back(m_downNeighbor->point);
	}

	api.lines(&vertices[0], vertices.size(), glm::vec4(1, 0, 0, 1), &pModelMat);
}
