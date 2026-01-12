#include "viewerbase.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "boids.h"

struct BoidViewer : Viewer {
	//--
	glm::vec2 mousePos;
	//--	
	bool leftMouseButtonPressed;
	bool altKeyPressed;

	Boids boids;

	BoidViewer();

	void init() override;
	void update(double elapsedTime) override;
	void render3D_Custom(const RenderApi3D& api) const override;
	void render3D_Skinning(const RenderApi3D& api) const override;

	void render3D(const RenderApi3D& api) const override;
	void render2D(const RenderApi2D& api) const override;
	void drawGUI() override;
};

