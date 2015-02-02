// pathfinder.h
// Trevor Westphal

#ifndef PathFinder_h
#define PathFinder_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <tuple>
#include <map>

#include "terrain.h"

struct Node{
	int x;
	int y;
	int g;
};

class PathFinder {
public:
	static std::vector<glm::vec3> find_path(Terrain*, int, int, int, int);
private:
	static float heuristic_estimate(int, int, int, int);
	static Node getLowestHeuristicEstimate(std::vector<Node>, int, int);
	
};

#endif