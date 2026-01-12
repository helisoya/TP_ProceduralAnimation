#include "boids.h"

#include "glm/vec4.hpp"

Boids::Boids(int amount)
{
	for (int i = 0; i < amount; i++) {
		boids.push_back(BoidEntity());
        boids[i].position += i*0.01f;
        boids[i].velocity = glm::vec3(rand() % 2 - 1, rand() % 2 - 1, rand() % 2 - 1);
	}
}

void Boids::Update(double elapsed_time)
{
    float neighboring_boids;
    float squaredDistance;
    float protected_range_squared = protectedRange*protectedRange;
    float visual_range_squared = visualRange * visualRange;

    glm::vec3 d;
    glm::vec3 close;
    glm::vec3 pos_avg;
    glm::vec3 vel_avg;

    auto currentIter = boids.begin();
    auto otherIter = boids.begin();
    while (currentIter != boids.end()) {

        squaredDistance = 0;
        neighboring_boids = 0;

        close.x = 0;
        close.y = 0;
        close.z = 0;
        pos_avg.x = 0;
        pos_avg.y = 0;
        pos_avg.z = 0;
        vel_avg.x = 0;
        vel_avg.y = 0;
        vel_avg.z = 0;

        otherIter = boids.begin();
        while (otherIter != boids.end()) {
            if (otherIter != currentIter) {
                d = currentIter->position - otherIter->position;

                if (abs(d.x) < visualRange && abs(d.y) < visualRange && abs(d.z) < visualRange) {
                    squaredDistance = d.x * d.x + d.y * d.y + d.z * d.z;
                    if (squaredDistance < protected_range_squared) {
                        close += d;
                    }
                    else if (squaredDistance < visual_range_squared) {
                        pos_avg += otherIter->position;
                        vel_avg += otherIter->velocity;
                        neighboring_boids++;

                    }
                }
            }
            
            otherIter++;
        }
        
        if (neighboring_boids > 0) {
            pos_avg /= neighboring_boids;
            vel_avg /= neighboring_boids;

            currentIter->velocity.x = (currentIter->velocity.x + 
                (pos_avg.x - currentIter->position.x) * centeringFactor + 
                (vel_avg.x - currentIter->velocity.x) * matchingFactor);

            currentIter->velocity.y = (currentIter->velocity.y +
                (pos_avg.y - currentIter->position.y) * centeringFactor +
                (vel_avg.y - currentIter->velocity.y) * matchingFactor);

            currentIter->velocity.z = (currentIter->velocity.z +
                (pos_avg.z - currentIter->position.z) * centeringFactor +
                (vel_avg.z - currentIter->velocity.z) * matchingFactor);
        }

        
        currentIter->velocity += close * avoidFactor;
        
        
        if (currentIter->position.x > range) {
            currentIter->velocity.x -= turnFactor;
        }
        else if (currentIter->position.x < -range) {
            currentIter->velocity.x += turnFactor;
        }

        if (currentIter->position.y > range) {
            currentIter->velocity.y -= turnFactor;
        }
        else if (currentIter->position.y < -range) {
            currentIter->velocity.y += turnFactor;
        }

        if (currentIter->position.z > range) {
            currentIter->velocity.z -= turnFactor;
        }
        else if (currentIter->position.z < -range) {
            currentIter->velocity.z += turnFactor;
        }
        
        float speed = sqrt(currentIter->velocity.x * currentIter->velocity.x +
            currentIter->velocity.y * currentIter->velocity.y +
            currentIter->velocity.z * currentIter->velocity.z);

        if (speed < minSpeed) {
            currentIter->velocity = currentIter->velocity / speed * minSpeed;
        }
        else if (speed > maxSpeed) {
            currentIter->velocity = currentIter->velocity / speed * maxSpeed;
        }

        currentIter->position += currentIter->velocity;

        currentIter++;
    }
}

void Boids::Draw(const RenderApi3D& api) const
{
    api.solidSphere(glm::vec3(range, range, range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, -range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(range, range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(range, -range, range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, range, range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(range, -range, -range), 0.2f, 10, 10, rangeColor);
    api.solidSphere(glm::vec3(-range, -range, range), 0.2f, 10, 10, rangeColor);

	auto iter = boids.begin();
	while (iter != boids.end()) {
		api.solidSphere(iter->position, 0.1f,10,10, boidColor);
		iter++;
	}
}
