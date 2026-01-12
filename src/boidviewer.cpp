#include "boidViewer.h"
#include "drawbuffer.h"
#include "renderapi.h"

#include <time.h>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>


#define COUNTOF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

constexpr static char const* viewerName = "Boids";
constexpr static glm::vec4 white = { 1.f, 1.f, 1.f, 1.f };
constexpr static glm::vec4 blue = { 0.f, 0.f, 1.f, 1.f };
constexpr static glm::vec4 green = { 0.f, 1.f, 0.f, 1.f };
constexpr static glm::vec4 red = { 1.f, 0.f, 0.f, 1.f };


BoidViewer::BoidViewer() : Viewer(viewerName, 1280, 720), boids(100){
	backgroundColor.g = 1;
}

void BoidViewer::init() {
	mousePos = { 0.f, 0.f };
	leftMouseButtonPressed = false;

	altKeyPressed = false;
}


void BoidViewer::update(double DeltaTime) {

	leftMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	altKeyPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	mousePos = { float(mouseX), viewportHeight - float(mouseY) };

	boids.Update(DeltaTime);
}

void BoidViewer::render3D_Custom(const RenderApi3D& api) const {
	//Here goes your drawcalls affected by the custom vertex shader
}

void BoidViewer::render3D_Skinning(const RenderApi3D& api) const{
	//Here goes your drawcalls affected by the skinning vertex shader
	//api.horizontalSkinnedCylinder({ 4, 2, 0 }, 4.f, 0.5f, 16, 50, glm::vec4(0.7f, 0.8f, 0.1f, 1.f), GetSkinningWeightCb, (void*)this);
}


void BoidViewer::render3D(const RenderApi3D& api) const {
	boids.Draw(api);
}

void BoidViewer::render2D(const RenderApi2D& api) const {
	
	constexpr float padding = 50.f;

	if (altKeyPressed) {
		if (leftMouseButtonPressed) {
			api.circleFill(mousePos, padding, 10, white);
		} else {
			api.circleContour(mousePos, padding, 10, white);
		}
			
	} else {
		const glm::vec2 min = mousePos + glm::vec2(padding, padding);
		const glm::vec2 max = mousePos + glm::vec2(-padding, -padding);
		if (leftMouseButtonPressed) {
			api.quadFill(min, max, white);
		}
		else {
			api.quadContour(min, max, white);
		}
	}

	{
		const glm::vec2 from = { viewportWidth * 0.5f, padding };
		const glm::vec2 to = { viewportWidth * 0.5f, 2.f * padding };
		constexpr float thickness = padding * 0.25f;
		constexpr float hatRatio = 0.3f;
		api.arrow(from, to, thickness, hatRatio, white);
	}

	{
		glm::vec2 vertices[] = {
			{ padding, viewportHeight - padding },
			{ viewportWidth * 0.5f, viewportHeight - 2.f * padding },
			{ viewportWidth * 0.5f, viewportHeight - 2.f * padding },
			{ viewportWidth - padding, viewportHeight - padding },
		};
		api.lines(vertices, COUNTOF(vertices), white);
	}
}

void BoidViewer::drawGUI() {
	static bool showDemoWindow = false;

	ImGui::Begin("Boids");

	ImGui::ColorEdit4("Background color", (float*)&backgroundColor, ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit4("Boid color", (float*)&boids.boidColor, ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit4("Cube color", (float*)&boids.rangeColor, ImGuiColorEditFlags_NoInputs);
	ImGui::Separator();


	ImGui::SliderFloat("Turn factor", &boids.turnFactor, 0.01f, 1.0f);
	ImGui::SliderFloat("Min Speed", &boids.minSpeed, 0.0001f, 1.0f);
	ImGui::SliderFloat("Max Speed", &boids.maxSpeed, 0.0001f, 1.0f);
	ImGui::Separator();

	ImGui::SliderFloat("Centering factor", &boids.centeringFactor, 0.01f, 1.0f);
	ImGui::SliderFloat("Matching factor", &boids.matchingFactor, 0.01f, 1.0f);
	ImGui::SliderFloat("Avoid factor", &boids.avoidFactor, 0.01f, 1.0f);
	ImGui::Separator();

	ImGui::SliderFloat("Protected Range", &boids.protectedRange, 0.01f, 1.0f);
	ImGui::SliderFloat("Visual Range", &boids.visualRange, 0.01f, 1.0f);
	ImGui::SliderFloat("Bounds", &boids.range, 0.01f, 10.0f);
	
	ImGui::Separator();
	ImGui::Separator();
	ImGui::SliderFloat("Line Width", &lineWidth, 0.1f, 10.f);
	ImGui::Separator();
	ImGui::SliderFloat3("Light dir", (float(&)[3])lightDir, -1.f, 1.f);
	ImGui::SliderFloat("Light Strength", &lightStrength, 0.f, 2.f);
	ImGui::SliderFloat("Ligh Ambient", &lightAmbient, 0.f, 0.5f);
	ImGui::SliderFloat("Ligh Specular", &specular, 0.f, 1.f);
	ImGui::SliderFloat("Ligh Specular Pow", &specularPow, 1.f, 200.f);
	ImGui::Separator();
	ImGui::Separator();
	float fovDegrees = glm::degrees(camera.fov);
	if (ImGui::SliderFloat("Camera field of fiew (degrees)", &fovDegrees, 15, 180)) {
		camera.fov = glm::radians(fovDegrees);
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	ImGui::End();

	if (showDemoWindow) {
		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}
