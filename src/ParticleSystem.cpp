#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
}

void ParticleSystem::AddWell()
{
    wells.push_back(Well());
}

void ParticleSystem::AddParticle()
{
    particles.push_back(Particle());
    particles[particles.size()-1].velocity = glm::vec3(rand() % 2 - 1, rand() % 2 - 1, rand() % 2 - 1);
}

void ParticleSystem::RemoveWell()
{
    if (wells.size() > 0) {
        wells.erase(wells.end() - 1);
    }
}

void ParticleSystem::RemoveParticle()
{
    if (particles.size() > 0) {
        particles.erase(particles.end() - 1);
    }
}

std::vector<Well>& ParticleSystem::GetWells()
{
    return wells;
}

std::vector<Particle>& ParticleSystem::GetParticles()
{
    return particles;
}

void ParticleSystem::Update(double elapsed_time)
{
    auto particleItr = particles.begin();
    auto wellItr = wells.begin();
    float distance;
    glm::vec3 distVec;
    while (particleItr != particles.end()) {

        particleItr->acceleration = glm::vec3(0, 0, 0);
        
        wellItr = wells.begin();
        while (wellItr != wells.end()) {

            distVec = wellItr->position - particleItr->position;
            distance = sqrt(distVec.x * distVec.x + distVec.y * distVec.y + distVec.z * distVec.z);
            if (distance <= 0.001f) distance = 0.001f;
            distVec /= distance;

            distVec *= wellItr->size / (distance * distance / 2.0f);
            particleItr->acceleration += distVec;

            wellItr++;
        }

        if (particleItr->position.x > range) {
            particleItr->velocity.x *= -1;
        }
        else if (particleItr->position.x < -range) {
            particleItr->velocity.x *= -1;
        }

        if (particleItr->position.y > range) {
            particleItr->velocity.y *= -1;
        }
        else if (particleItr->position.y < -range) {
            particleItr->velocity.y *= -1;
        }

        if (particleItr->position.z > range) {
            particleItr->velocity.z *= -1;
        }
        else if (particleItr->position.z < -range) {
            particleItr->velocity.z *= -1;
        }
        
        float speed = sqrt(particleItr->velocity.x * particleItr->velocity.x +
            particleItr->velocity.y * particleItr->velocity.y +
            particleItr->velocity.z * particleItr->velocity.z);

        if (speed < minSpeed) {
            particleItr->velocity = particleItr->velocity / speed * minSpeed;
        }
        else if (speed > maxSpeed) {
            particleItr->velocity = particleItr->velocity / speed * maxSpeed;
        }

        particleItr->velocity += particleItr->acceleration * (float)elapsed_time;
        particleItr->position += particleItr->velocity * (float)elapsed_time;

        particleItr++;
    }
}

void ParticleSystem::Draw(const RenderApi3D& api) const
{
    api.solidSphere(glm::vec3(range, range, range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, -range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(range, range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(range, -range, range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, range, range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(range, -range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, -range, range), 0.2f, 10, 10, rangeColor);

    auto iter = wells.begin();
    while (iter != wells.end()) {
        api.solidSphere(iter->position, 0.2f, 10, 10, wellColor);
        iter++;
    }

    auto iter2 = particles.begin();
    while (iter2 != particles.end()) {
        api.solidSphere(iter2->position, 0.1f, 10, 10, particleColor);
        iter2++;
    }
}