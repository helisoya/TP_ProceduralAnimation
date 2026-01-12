#pragma once

#include "renderapi.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "boidentity.h"




struct Boids {
public:
	float turnFactor = 0.025f;
	float minSpeed = 0.001f;
	float maxSpeed = 0.05f;
	float centeringFactor = 0.1f;
	float matchingFactor = 0.1f;
	float avoidFactor = 0.5f;
	float protectedRange = 0.05f;
	float visualRange = 0.1f;
	float range = 5.0f;
	glm::vec4 boidColor = { 0.f, 0.f, 1.f, 1.f };
	glm::vec4 rangeColor = { 1.f, 0.f, 0.f, 1.f };
	
private:
	std::vector<BoidEntity> boids;

public:

	Boids(int amount);

	void Update(double elapsed_time);
	void Draw(const RenderApi3D& api) const;
};

