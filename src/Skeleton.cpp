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
	std::vector<float> distanceToPrevious;
	float totalDistance = 0.0f;
	float distTmp;
	float lambda;

	glm::vec3 absolutePosition = { 0,0,0 };
	glm::quat absoluteRotation = glm::quat(glm::orientate3(glm::vec3(0, 0, 0)));
	for (int i = 0; i < bones.size(); i++) {

		absoluteRotation = absoluteRotation * bones[i].relativeRotation;
		distTmp = glm::distance(absolutePosition, absolutePosition + absoluteRotation * bones[i].relativePosition);
		absolutePosition += absoluteRotation * bones[i].relativePosition;

		totalDistance += distTmp;
		distanceToPrevious.push_back(distTmp);
		absolutePositions.push_back(absolutePosition);
		absoluteRotations.push_back(absoluteRotation);
	}

	float dist = glm::distance(absolutePositions[0], target);
	
	if (dist > totalDistance) {
		// cannot reach
		for (int i = 0; i < bones.size()-1; i ++) {
			distTmp = glm::distance(absolutePositions[i],target);
			lambda = distanceToPrevious[i] / distTmp;

			// 0.0f is lambdaT
			absolutePositions[i + 1] = (1 - lambda) * absolutePositions[i] + lambda * target;
		}
	}
	else {
		// can reach target
		glm::vec3 rootStart = absolutePositions[0];
		float accuracy = glm::distance2(*(--absolutePositions.end()), target);

		float tolerance = 1;

		while (accuracy > tolerance)
		{
			absolutePositions[absolutePositions.size() - 1] = target;
			for (int loop = absolutePositions.size() - 2; loop >= 0; loop--)
			{
				distTmp = glm::distance(absolutePositions[loop+1], absolutePositions[loop]);
				lambda = distanceToPrevious[loop] / distTmp;

				// 0.0f is lambdaT
				absolutePositions[loop + 1] = (1 - lambda) * absolutePositions[loop+1] + lambda * absolutePositions[loop];
			}

			absolutePositions[0] = rootStart;

			for (int loop = 1; loop < absolutePositions.size(); loop++)
			{
				distTmp = glm::distance(absolutePositions[loop + 1], absolutePositions[loop]);
				lambda = distanceToPrevious[loop] / distTmp;

				// 0.0f is lambdaT
				absolutePositions[loop + 1] = (1 - lambda) * absolutePositions[loop] + lambda * absolutePositions[loop + 1];
			}

			accuracy = glm::distance2(*(--absolutePositions.end()), target);
		}
	}

	absolutePosition = { 0,0,0 };
	absoluteRotation = glm::quat(glm::orientate3(glm::vec3(0, 0, 0)));
	for (int i = 0; i < bones.size(); i++) {

		bones[i].relativePosition = absolutePositions[i] - absolutePosition;
		absolutePosition += bones[i].relativePosition;
		bones[i].relativeRotation = absoluteRotation;
	}
}
