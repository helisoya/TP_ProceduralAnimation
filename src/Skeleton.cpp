#include "Skeleton.h"

Skeleton::Skeleton()
{
	AddBone();
	AddBone();
}

void Skeleton::Update(double elapsed_time)
{
}

void Skeleton::Draw(const RenderApi3D& api) const
{
	glm::quat absoluteRotation = glm::quat(glm::orientate3(glm::vec3(0,0,0)));
	glm::vec3 absolutePosition = { 0,0,0 };

	auto itr = bones.begin();
	while (itr != bones.end()) {

		api.bone(itr->relativeRotation * itr->relativePosition, boneColor, absoluteRotation, absolutePosition);
		
		absoluteRotation = absoluteRotation * itr->relativeRotation;
		absolutePosition += absoluteRotation * itr->relativePosition;

		api.solidSphere(absolutePosition, 0.1f, 10, 10, boneColor);

		itr++;
	}
}

std::vector<Bone>& Skeleton::GetBones()
{
	return bones;
}

void Skeleton::AddBone()
{
	bones.push_back(Bone());
}

void Skeleton::RemoveBone()
{
	bones.pop_back();
}
