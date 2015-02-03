// pathfinder.h
// Trevor Westphal

#ifndef PathFinder_h
#define PathFinder_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <tuple>
#include <map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector

#include "terrain.h"

struct Node{
	int x;
	int y;
	int g;
};

struct LessThanByGScore{
  bool operator()(const Node& lhs, const Node& rhs) const {
  	// Reverse ordering for lowest
    return lhs.g > rhs.g;
  }
};

class PathFinder {
public:
	static std::vector<glm::vec3> find_path(Terrain*, int, int, int, int);
private:
	static float heuristic_estimate(int, int, int, int);
	static std::vector<glm::vec3> reconstruct_path(std::map<Node, Node>, Node);
};

#endif