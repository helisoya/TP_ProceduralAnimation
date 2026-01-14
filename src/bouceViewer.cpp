#include "bounceViewer.h"
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

constexpr static char const* viewerName = "MyViewer";
constexpr static glm::vec4 white = { 1.f, 1.f, 1.f, 1.f };
constexpr static glm::vec4 blue = { 0.f, 0.f, 1.f, 1.f };
constexpr static glm::vec4 green = { 0.f, 1.f, 0.f, 1.f };
constexpr static glm::vec4 red = { 1.f, 0.f, 0.f, 1.f };


BounceViewer::BounceViewer() : Viewer(viewerName, 1280, 720) {
	decaySpeed = 1.0f;
	currentBounceIndex = 0;
	strength = 1.1f;
	distanceFactor = 1.0f;
}

void BounceViewer::init() {
	mousePos = { 0.f, 0.f };
	leftMouseButtonPressed = false;

	altKeyPressed = false;

	additionalShaderData.Pos = { 0., 0., 0., 0. };
	backgroundColor = { 0, 1, 0, 1 };

	for (int i = 0; i < MaxBounces; i++) {
		additionalShaderData.bouncesData[i] = { strength, 0.0f, distanceFactor, 0.0f};
		additionalShaderData.bouncesPosition[i] = { 0,0,0,0 };
	}
}


void BounceViewer::update(double DeltaTime) {

	leftMouseButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	altKeyPressed = glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	mousePos = { float(mouseX), viewportHeight - float(mouseY) };

	pCustomShaderData = &additionalShaderData;
	CustomShaderDataSize = sizeof(VertexShaderAdditionalDataBounce);

	static float Something = 0.5;
	pSkinningShaderData = &Something;
	SkinningShaderDataSize = sizeof(Something);

	for (int i = 0; i < MaxBounces; i++) {
		if (additionalShaderData.bouncesData[i].y > 0) {
			additionalShaderData.bouncesData[i].y = glm::max(additionalShaderData.bouncesData[i].y - (float)DeltaTime * decaySpeed, 0.0f);
		}
	}
}

void BounceViewer::render3D_Custom(const RenderApi3D& api) const {
	//Here goes your drawcalls affected by the custom vertex shader
	api.horizontalPlane({ 0, 0, 0 }, { 4, 4 }, 200, glm::vec4(0.0f, 0.2f, 1.f, 1.f));
}

void BounceViewer::render3D_Skinning(const RenderApi3D& api) const {
	//Here goes your drawcalls affected by the skinning vertex shader
	//api.horizontalSkinnedCylinder({ 4, 2, 0 }, 4.f, 0.5f, 16, 50, glm::vec4(0.7f, 0.8f, 0.1f, 1.f), GetSkinningWeightCb, (void*)this);
}


void BounceViewer::render3D(const RenderApi3D& api) const {
}

void BounceViewer::render2D(const RenderApi2D& api) const {

	constexpr float padding = 50.f;

	if (altKeyPressed) {
		if (leftMouseButtonPressed) {
			api.circleFill(mousePos, padding, 10, white);
		}
		else {
			api.circleContour(mousePos, padding, 10, white);
		}

	}
	else {
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

void BounceViewer::drawGUI() {
	static bool showDemoWindow = false;

	ImGui::Begin("3D Sandbox");

	ImGui::ColorEdit4("Background color", (float*)&backgroundColor, ImGuiColorEditFlags_NoInputs);

	ImGui::Separator();
	ImGui::DragFloat4("Position", (float(&)[4])additionalShaderData.Pos, 0.1f, -10.f, 10.f);
	ImGui::DragFloat("Strenth", &strength, 0.1f, 0.0f, 10.f);
	ImGui::DragFloat("Distance factor", &distanceFactor, 0.1f,0.0f, 1.f);
	ImGui::Separator();

	ImGui::DragFloat("Decay speed", &decaySpeed, 0.1f, 0.0f, 1.f);
	ImGui::Separator();

	if (ImGui::Button("Random bounce")) {

		additionalShaderData.bouncesData[currentBounceIndex].y = strength;
		additionalShaderData.bouncesData[currentBounceIndex].y = 1.0f;
		additionalShaderData.bouncesData[currentBounceIndex].z = distanceFactor;
		additionalShaderData.bouncesPosition[currentBounceIndex] = glm::vec4(rand() % 4 - 2, 0, rand() % 4 - 2, 0);
		currentBounceIndex = (currentBounceIndex + 1) % MaxBounces;
			
		//additionalShaderData.bounceData.y = 1.0f;
		//additionalShaderData.bouncePosition = glm::vec4(rand() % 4 - 2, 0, rand() % 4 - 2, 0);
	}

	ImGui::SliderFloat("Point size", &pointSize, 0.1f, 10.f);
	ImGui::SliderFloat("Line Width", &lineWidth, 0.1f, 10.f);
	ImGui::Separator();
	ImGui::SliderFloat3("Light dir", (float(&)[3])lightDir, -1.f, 1.f);
	ImGui::SliderFloat("Light Strength", &lightStrength, 0.f, 2.f);
	ImGui::SliderFloat("Ligh Ambient", &lightAmbient, 0.f, 0.5f);
	ImGui::SliderFloat("Ligh Specular", &specular, 0.f, 1.f);
	ImGui::SliderFloat("Ligh Specular Pow", &specularPow, 1.f, 200.f);
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
