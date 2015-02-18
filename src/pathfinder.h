// pathfinder.h
// Trevor Westphal

#ifndef PathFinder_h
#define PathFinder_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>  // std::unordered_map
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <algorithm>	  // std::swap

#include "terrain.h"

class Node{
public:
	Node(int x, int y, int g){
		this->x = x;
		this->y = y;
		this->g = g;
	}

	int x;
	int y;
	float g;
};

struct LessThanByGScore{
  bool operator()(const Node* lhs, const Node* rhs) const {
  	// Reverse ordering for lowest
    return lhs->g > rhs->g;
  }
};

class PathFinder {
public:
	static void allocateArray(Terrain*);
	static std::vector<glm::vec3> find_path(Terrain*, float, float, float, float);
private:
	static float distance_between(int, int, int, int);
	static float heuristic_estimate(int, int, int, int);
	static std::vector<glm::vec3> reconstruct_path(Terrain*, std::unordered_map<Node*, Node*>, Node*);
	static std::vector<Node*> getNeighborNodes(Node*);
	static bool canPathOnLine(Terrain*, float, float, float, float);

	static int **node_state_array;
	static int depth;
	static int width;

	static const int UNVISITED = 0;
	static const int IN_FRONTIER = 1;
	static const int VISITED = 2;
};

#endif