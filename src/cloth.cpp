#include "cloth.h"

#include <imgui.h>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/projection.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>


Cloth::Cloth() : Viewer("Cloth", 1000, 1000), basePos(0, 0, 0)
{
}

void Cloth::init()
{
	mousePos = { 0.f, 0.f };
	leftMouseButtonPressed = false;

	altKeyPressed = false;

	for (int loop = 0; loop < m_points.size(); loop++)
	{
		glm::vec3& point = m_points[loop].point;

		float xDiff = COLUMN_SEPARATION * (loop % COLUMNS);
		float yDiff = LINES_SEPARATION * (loop / COLUMNS);

		point = glm::vec3(1, 0, 0) * xDiff - glm::vec3(0, 1, 0) * yDiff;

		if (loop < COLUMNS && loop % 2 == 0)
		{
			m_points[loop].shouldMove = false;
		}

		ClothPoint* leftNeighbor = nullptr;
		ClothPoint* upNeighbor = nullptr;
		
		if ((loop % COLUMNS) - 1 >= 0)
		{
			leftNeighbor = &m_points[loop - 1];
		}

		if (loop >= COLUMNS)
		{
			upNeighbor = &m_points[loop - COLUMNS];
		}

		m_points[loop].Init(leftNeighbor, upNeighbor);
	}
}

void Cloth::update(double deltaTime)
{
	static float Something = 0.5;
	pSkinningShaderData = &Something;
	SkinningShaderDataSize = sizeof(Something);

	bool wasLeftMouseButtonPressed = leftMouseButtonPressed && !altKeyPressed;
	leftMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	altKeyPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	mousePos = { float(mouseX), viewportHeight - float(mouseY) };

	// mouse interaction
	glm::mat4 projection = glm::perspective(camera.fov, viewportWidth / float(viewportHeight), 0.1f, 100.f);
	glm::mat4 view = glm::lookAt(camera.eye, camera.o, camera.up);
	glm::mat4 viewRot = glm::lookAt(glm::vec3(0, 0, 0), camera.o - camera.eye, camera.up);

	if (!altKeyPressed && leftMouseButtonPressed)
	{
		if (wasLeftMouseButtonPressed)
		{
			// Keep same point if still clicking
			glm::vec2 mousePosView(mousePos.x / viewportWidth, mousePos.y / viewportHeight);
			mousePosView = glm::vec2((mousePosView.x - 0.5f) * 2, (mousePosView.y - 0.5f) * 2);
			glm::vec4 mousePosScreen(mousePosView.x, mousePosView.y, 0.8, 1.0f);
			mousePosWorld = glm::vec3(glm::inverse(projection * view) * mousePosScreen);

			float distanceMouseToCam = glm::distance(camera.eye, mousePosWorld);
			float distancePointToCam = glm::distance(camera.eye, closestPoint->point);
			mousePosWorld *= (distancePointToCam / distanceMouseToCam);
		}
		else
		{
			// New point to track
			closestPoint = nullptr;
			float minDistance = FLT_MAX;

			for (int loop = 0; loop < m_points.size(); loop++)
			{
				ClothPoint& clothPoint = m_points[loop];

				if (!clothPoint.shouldMove)
					continue;

				glm::vec2 mousePosView(mousePos.x / viewportWidth, mousePos.y / viewportHeight);
				mousePosView = glm::vec2((mousePosView.x - 0.5f) * 2, (mousePosView.y - 0.5f) * 2);
				glm::vec4 mousePosScreen(mousePosView.x, mousePosView.y, 0.8, 1.0f);
				mousePosWorld = glm::vec3(glm::inverse(projection * view) * mousePosScreen);

				float distanceMouseToCam = glm::distance(camera.eye, mousePosWorld);
				float distancePointToCam = glm::distance(camera.eye, clothPoint.point);
				mousePosWorld *= (distancePointToCam / distanceMouseToCam);

				glm::vec3 distanceVec = glm::cross(glm::normalize(camera.o - camera.eye), clothPoint.point - mousePosWorld);
				float distance = glm::length(distanceVec);


				if (distance < minDistance)
				{
					closestPoint = &clothPoint;
					minDistance = distance;
				}
			}
		}
	}
	else
	{
		closestPoint = nullptr;
	}


	for (int loop = 0; loop < m_points.size(); loop++)
	{
		ClothPoint& clothPoint = m_points[loop];

		// User Input calculations
		if (closestPoint != nullptr && closestPoint == &clothPoint)
		{
			// dont use raw mousePos			
			glm::vec3 fwd = camera.eye - camera.o;
			glm::vec3 left = glm::cross(fwd, glm::vec3(0, 1, 0));
			glm::vec3 up = glm::cross(left, fwd);

			glm::vec3 upMousePos = glm::proj(mousePosWorld, up);
			glm::vec3 leftMousePos = glm::proj(mousePosWorld, left);
			clothPoint.point = leftMousePos + upMousePos;
		}
		
		//Point force calculations
		clothPoint.CalculateForces(gravity, baseDistance, stifness, deltaTime);
	}

	for (int loop = 0; loop < m_points.size(); loop++)
	{
		m_points[loop].ApplyForces();
	}
}

void Cloth::render3D_Custom(const RenderApi3D& api) const
{
}

void Cloth::render3D_Skinning(const RenderApi3D& api) const
{
}

void Cloth::render3D(const RenderApi3D& api) const
{
	auto itr = m_points.begin();
	while (itr != m_points.end())
	{
		itr->Render(api);
		itr++;
	}

	//api.solidSphere(glm::vec3(mousePosWorld), 0.1f, 4, 4, glm::vec4(0, 0, 1, 1));
}

void Cloth::render2D(const RenderApi2D& api) const
{
}

void Cloth::drawGUI()
{
	static bool showDemoWindow = false;

	ImGui::Begin("Cloth");

	ImGui::SliderFloat3("Base Position", (float(&)[3])basePos, -10.f, 10.f);
	ImGui::InputFloat("Gravity", &gravity);
	ImGui::InputFloat("Base Distance", &baseDistance);
	ImGui::InputFloat("Stifness", &stifness);

	ImGui::End();

	
}


