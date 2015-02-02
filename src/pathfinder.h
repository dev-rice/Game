// pathfinder.h
// Trevor Westphal

#ifndef PathFinder_h
#define PathFinder_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <tuple>

#include "terrain.h"

class PathFinder {
public:
	static std::vector<glm::vec3> find_path(Terrain*, int, int, int, int);
private:
	
};

#endif