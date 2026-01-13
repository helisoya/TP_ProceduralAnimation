#pragma once

#include "renderapi.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

struct Bone {
	glm::vec3 relativePosition = { 0,1,0 };
	glm::quat relativeRotation = glm::quat(glm::orientate3(glm::vec3(0,0,0)));
	glm::vec3 tempEulers = { 0,0,0 };
};



struct Skeleton {
public:
	glm::vec4 boneColor = { 0.f, 0.f, 1.f, 1.f };

private:
	std::vector<Bone> bones;

public:

	Skeleton();

	void Update(double elapsed_time);
	void Draw(const RenderApi3D& api) const;

	std::vector<Bone>& GetBones();
	void AddBone();
	void RemoveBone();
};
