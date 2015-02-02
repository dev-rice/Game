// pathfinder.h
// Trevor Westphal

#ifndef PathFinder_h
#define PathFinder_h

#include <vector>

#include "playable.h"
#include "terrain.h"

class PathFinder {
public:
	static std::vector<std::tuple<Playable::Order, glm::vec3>> find_path(terrain*, int, int ,int , int);
};

#endif