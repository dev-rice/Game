// pathfinder.cpp
// Trevor Westphal

#include "pathfinder.h"

std::vector<glm::vec3> PathFinder::find_path(Terrain *ground, int start_x, int start_y, int target_x, int target_y){
	printf("Beginning A* search...\n");
	// could put some benchmarking code here

	// Setup
	std::vector<Node> visited_nodes;
	std::vector<Node> frontier_nodes; 
	Node start_node = {start_x, start_y, 0};
	frontier_nodes.push_back(start_node);
	std::map<Node, Node> parent_of;
	float f_score = start_node.g + heuristic_estimate(start_node.x, start_node.y, target_x, target_y);

 	while( ! frontier_nodes.empty()){
 		 Node current_node = getLowestHeuristicEstimate(frontier_nodes, target_x, target_y);


 		 break;
 	}



 	printf("Finished A* search.\n");

	std::vector<glm::vec3> temp;
	return temp;
}

float PathFinder::heuristic_estimate(int current_x, int current_y, int target_x, int target_y){
	// Heuristic function
	int x_delta = abs(current_x - target_x);
	int y_delta = abs(current_y - target_y);

	return sqrt(float(x_delta*x_delta + y_delta*y_delta));
}

Node PathFinder::getLowestHeuristicEstimate(std::vector<Node> list, int target_x, int target_y){
	float max = -9999.0f;
	Node max_node;
	for(int i = 0; i < list.size(); ++i){
		float f_score = list[i].g + heuristic_estimate(list[i].x, list[i].y, target_x, target_y);
		if( f_score > max){
			max = f_score;
			max_node = list[i];
		}
	}

	return max_node;
}