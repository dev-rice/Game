// pathfinder.cpp
// Trevor Westphal

#include "pathfinder.h"

std::vector<glm::vec3> PathFinder::find_path(Terrain *ground, int start_x, int start_y, int target_x, int target_y){

//         add current to closedset
//         for each neighbor in neighbor_nodes(current)
//             if neighbor in closedset
//                 continue
//             tentative_g_score := g_score[current] + dist_between(current,neighbor)
 
//             if neighbor not in openset or tentative_g_score < g_score[neighbor] 
//                 came_from[neighbor] := current
//                 g_score[neighbor] := tentative_g_score
//                 f_score[neighbor] := g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)
//                 if neighbor not in openset
//                     add neighbor to openset
 
//     return failure
 
// function reconstruct_path(came_from,current)
//     total_path := [current]
//     while current in came_from:
//         current := came_from[current]
//         total_path.append(current)
//     return total_path

	printf("Beginning A* search... ");
	// could put some benchmarking code here

	std::map<Node*, bool> visited_nodes;											// The set of nodes already evaluated.
	std::priority_queue<Node*, std::vector<Node*>, LessThanByGScore> frontier_nodes;	// The set of tentative nodes to be evaluated...
				
	frontier_nodes.push(new Node(start_x, start_y, 0));								// ...initially containing the start node
																					// Cost from start along best known path (included)
	
	std::map<Node*, Node*> parent_of;												// The map of navigated nodes.

	float f_score = heuristic_estimate(start_x, start_y, target_x, target_y);
																					// Estimated total cost from start to goal through y.

 	while( ! frontier_nodes.empty()){
 	
 		Node* current_node = frontier_nodes.top();

        if(current_node->x == target_x && current_node->y == target_y){
        	return reconstruct_path(parent_of, current_node);
        }

        frontier_nodes.pop();

        visited_nodes[current_node] = true;

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

std::vector<glm::vec3> PathFinder::reconstruct_path(std::map<Node*, Node*> parent_of, Node* origin){
	std::vector<glm::vec3> temp;
	return temp;
}
