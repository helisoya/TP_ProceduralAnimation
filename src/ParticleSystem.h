#pragma once

#include "renderapi.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>

struct Well {
	glm::vec3 position;
	float size = 1.0f;
};

struct Particle {
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct ParticleSystem {
public:
	std::vector<Well> wells;
	std::vector<Particle> particles;

	float range = 10.0f;
	float minSpeed = 5.0f;
	float maxSpeed = 8.0f;
	glm::vec4 rangeColor = { 1.f, 0.f, 0.f, 1.f };
	glm::vec4 wellColor = { 0.f, 0.f, 1.f, 1.f };
	glm::vec4 particleColor = { 1.f, 0.f, 1.f, 1.f };

public:

	ParticleSystem();

	void AddWell();
	void AddParticle();

	void RemoveWell();
	void RemoveParticle();

	void Update(double elapsed_time);
	void Draw(const RenderApi3D& api) const;

	std::vector<Well>& GetWells();
	std::vector<Particle>& GetParticles();
};