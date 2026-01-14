#include "viewer.h"
#include "boidviewer.h"
#include "particleViewer.h"
#include "skeletonviewer.h"
#include "bounceViewer.h"

int main(int argc, char** argv) {
	BounceViewer v;
	return v.run();
}
