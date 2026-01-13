#pragma once
#include <glm/vec3.hpp>
#include "renderapi.h"
#include <glm/ext/matrix_transform.hpp>

struct ClothPoint
{
private:
	ClothPoint* m_leftNeighbor = nullptr;
	ClothPoint* m_downNeighbor = nullptr;

	glm::mat4 pModelMat = glm::identity<glm::mat4>();

	void CalculateNeighborForce(ClothPoint* neighbor, const float gravity, const float baseDistance, const float stifness, const double deltaTime);

public:
	glm::vec3 point;
	glm::vec3 force = glm::vec3(0, 0, 0);

	bool shouldMove = true;

	void Init(ClothPoint* leftNeighbor, ClothPoint* upNeighbor);
	void CalculateForces(const float gravity, const float baseDistance, const float stifness, const double deltaTime);
	void ApplyForces();
	void Render(const RenderApi3D& api) const;
};