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

	api.solidSphere(ikTarget, 0.1f, 10, 10, ikTargetColor);
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

void Skeleton::FabrikIK()
{
	if (bones.size() == 0) return;

	FabrikIteration(ikTarget);
}

void Skeleton::FabrikIteration(glm::vec3 target)
{
	std::vector<glm::vec3> absolutePositions;
	std::vector<glm::quat> absoluteRotations;
	std::vector<float> distanceToNext;
	float totalDistance = 0.0f;
	float distTmp;
	float lambda;

	absolutePositions.push_back({ 0, 0, 0 });
	absoluteRotations.push_back({ 0, 0, 0, 1 });

	glm::vec3 absolutePosition = { 0,0,0 };
	glm::quat absoluteRotation = glm::quat(glm::orientate3(glm::vec3(0, 0, 0)));
	for (int i = 0; i < bones.size(); i++) {

		absoluteRotation = absoluteRotation * bones[i].relativeRotation;
		distTmp = glm::distance(absolutePosition, absolutePosition + absoluteRotation * bones[i].relativePosition);
		absolutePosition += absoluteRotation * bones[i].relativePosition;

		totalDistance += distTmp;
		distanceToNext.push_back(distTmp);
		absolutePositions.push_back(absolutePosition);
		absoluteRotations.push_back(absoluteRotation);
	}

	for (int loop = 0; loop < absolutePositions.size() - 1; loop++)
	{
		distanceToNext.push_back(glm::distance(absolutePositions[loop], absolutePositions[loop + 1]));
	}

	float dist = glm::distance(absolutePositions[0], target);
	
	if (dist > totalDistance) {
		// cannot reach
		for (int i = 0; i < absolutePositions.size()-1; i ++) {
			distTmp = glm::distance(absolutePositions[i], target);
			lambda = distanceToNext[i] / distTmp;

			absolutePositions[i + 1] = (1 - lambda) * absolutePositions[i] + lambda * target;
		}
	}
	else {
		// can reach target
		glm::vec3 rootStart = absolutePositions[0];
		float accuracy = glm::distance2(*(absolutePositions.rbegin()), target);

		while (accuracy > tolerance)
		{
			absolutePositions[absolutePositions.size() - 1] = target;
			for (int loop = absolutePositions.size() - 1; loop > 0; loop--)
			{
				distTmp = glm::distance(absolutePositions[loop - 1], absolutePositions[loop]);

				lambda = distanceToNext[loop] / distTmp;

				absolutePositions[loop - 1] = (1 - lambda) * absolutePositions[loop] + lambda * absolutePositions[loop - 1];
			}

			absolutePositions[0] = rootStart;

			for (int loop = 0; loop < absolutePositions.size() - 1; loop++)
			{
				distTmp = glm::distance(absolutePositions[loop + 1], absolutePositions[loop]);
				lambda = distanceToNext[loop] / distTmp;

				absolutePositions[loop + 1] = (1 - lambda) * absolutePositions[loop] + lambda * absolutePositions[loop + 1];
			}

			accuracy = glm::distance2(*(absolutePositions.rbegin()), target);
		}
	}

	absolutePosition = absolutePositions[0];
	absoluteRotation = glm::quat(glm::orientate3(glm::vec3(0, 0, 0)));
	for (int i = 0; i < bones.size(); i++) {

		bones[i].relativePosition = absolutePositions[i+1] - absolutePosition;
		absolutePosition += bones[i].relativePosition;
		bones[i].relativeRotation = absoluteRotation;
	}
}
