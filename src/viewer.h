#include "viewerbase.h"
#include "cloth.h"


#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct VertexShaderAdditionalData {
	glm::vec4 Pos;
	/// beware of alignement (std430 rule)
};


struct MyViewer : Viewer {
	//--
	glm::vec3 jointPosition;
	glm::vec3 cubePosition;
	float boneAngle;
	//--
	glm::vec2 mousePos;
	//--	
	bool leftMouseButtonPressed;
	bool altKeyPressed;
	//--
	VertexShaderAdditionalData additionalShaderData;

	MyViewer();

	void init() override;
	void update(double elapsedTime) override;
	void render3D_Custom(const RenderApi3D& api) const override;
	void render3D_Skinning(const RenderApi3D& api) const override;

	void render3D(const RenderApi3D& api) const override;
	void render2D(const RenderApi2D& api) const override;
	void drawGUI() override;
};

