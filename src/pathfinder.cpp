// pathfinder.cpp
// Trevor Westphal

#include "pathfinder.h"

std::vector<glm::vec3> PathFinder::find_path(Terrain *ground, int start_x, int start_y, int target_x, int target_y){

//             if neighbor not in openset or tentative_g_score < g_score[neighbor] 
//                 came_from[neighbor] := current
//                 g_score[neighbor] := tentative_g_score
//                 f_score[neighbor] := g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)


	printf("Beginning A* search...\n");
	// could put some benchmarking code here

	std::map<Node*, bool> visited_nodes;											// The set of nodes already evaluated.
	
	std::priority_queue<Node*, std::vector<Node*>, LessThanByGScore> frontier_nodes;// The set of tentative nodes to be evaluated...
			
	Node *start_node = new Node(start_x, start_y, 0.0f);							// ...initially containing the start node		
	frontier_nodes.push(start_node);												// Cost from start along best known path (included)							                
												
	std::map<Node*, Node*> parent_of;												// The map of navigated nodes.

	float f_score = heuristic_estimate(start_x, start_y, target_x, target_y);
																					// Estimated total cost from start to goal through y.

 	while( ! frontier_nodes.empty()){
 	
 		Node* current_node = frontier_nodes.top();

        if(current_node->x == target_x && current_node->y == target_y){
        	printf("Finished A* search.\n");
        	return reconstruct_path(parent_of, current_node);
        }

        frontier_nodes.pop();

        visited_nodes[current_node] = true;

        std::vector<Node*> neighbor_nodes = getNeighborNodes(current_node);

        for(int i = 0; i < neighbor_nodes.size(); ++i){

        	float temp_g_score = current_node->g + distance_between(current_node->x, current_node->y, neighbor_nodes[i]->x, neighbor_nodes[i]->y);

        	bool is_not_visited = !visited_nodes[neighbor_nodes[i]];
        	bool is_not_in_frontier = ! nodeIsInList(frontier_nodes, neighbor_nodes[i]);

        	if(is_not_visited && (is_not_in_frontier || temp_g_score < neighbor_nodes[i]->g)){
        		parent_of[neighbor_nodes[i]] = current_node;
        		neighbor_nodes[i]->g = temp_g_score;

        		if(is_not_in_frontier){
        			frontier_nodes.push(neighbor_nodes[i]);
        		}
        	}
        }

 		break;
 	}

 	// Failure reaches here
	std::vector<glm::vec3> temp;
	return temp;
}

float PathFinder::distance_between(int current_x, int current_y, int target_x, int target_y){
	int x_delta = abs(current_x - target_x);
	int y_delta = abs(current_y - target_y);

	return sqrt(float(x_delta*x_delta + y_delta*y_delta));
}


float PathFinder::heuristic_estimate(int a, int b, int c, int d){
	// Heuristic function
	return distance_between(a, b, c, d);
}

std::vector<glm::vec3> PathFinder::reconstruct_path(std::map<Node*, Node*> parent_of, Node* origin){
	std::vector<glm::vec3> temp;
	temp.push_back(glm::vec3(origin->x, 0.0f, origin->y));

	while(parent_of[origin]){
		origin = parent_of[origin];
		temp.push_back(glm::vec3(origin->x, 0.0f, origin->y));
	}

	return temp;
}

std::vector<Node*> PathFinder::getNeighborNodes(Node *current_node){
	std::vector<Node*> temp;
	temp.push_back(new Node(current_node->x + 1, current_node->y - 1, current_node->g + 1.4f));
	temp.push_back(new Node(current_node->x + 1, current_node->y + 0, current_node->g + 1.0f));
	temp.push_back(new Node(current_node->x + 1, current_node->y + 1, current_node->g + 1.4f));
	temp.push_back(new Node(current_node->x + 0, current_node->y - 1, current_node->g + 1.0f));
	temp.push_back(new Node(current_node->x + 0, current_node->y + 1, current_node->g + 1.0f));
	temp.push_back(new Node(current_node->x - 1, current_node->y - 1, current_node->g + 1.4f));
	temp.push_back(new Node(current_node->x - 1, current_node->y + 0, current_node->g + 1.0f));
	temp.push_back(new Node(current_node->x - 1, current_node->y + 1, current_node->g + 1.4f));
	return temp;
}

bool PathFinder::nodeIsInList(std::priority_queue<Node*, std::vector<Node*>, LessThanByGScore> list, Node* node){
	while(! list.empty()){
		Node* temp = list.top();
		list.pop();

		if((node->x == temp->x) && (node->y == temp->y)){
			return true;
		}
	}
	return false;
}