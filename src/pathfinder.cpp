// pathfinder.cpp
// Trevor Westphal

#include "pathfinder.h"

std::vector<glm::vec3> PathFinder::find_path(Terrain *ground, int start_x, int start_y, int target_x, int target_y){

	printf("Beginning A* search...\n");
	// could put some benchmarking code here

	// OCCASIONALLY SEGFAULTS... WHY!?!?!?!


	// THIS CAN BE MORE EFFECIENT
	std::vector<Node*> visited_nodes;											// The set of nodes already evaluated.
	
	std::priority_queue<Node*, std::vector<Node*>, LessThanByGScore> frontier_nodes;// The set of tentative nodes to be evaluated...
			
	Node *start_node = new Node(start_x, start_y, 0.0f);							// ...initially containing the start node		
	frontier_nodes.push(start_node);												// Cost from start along best known path (included)							                
												
	std::map<Node*, Node*> parent_of;												// The map of navigated nodes.
																					
	int count = 0;

 	while( ! frontier_nodes.empty()){
 		count ++;
 		Node* current_node = frontier_nodes.top();

        if(current_node->x == target_x && current_node->y == target_y){
   			printf("Final count: %d\n", count);
        	printf("Finished A* search successfully.\n");
        	return reconstruct_path(parent_of, current_node);
        }

        frontier_nodes.pop();

        visited_nodes.push_back(current_node);

        std::vector<Node*> neighbor_nodes = getNeighborNodes(current_node);

        for(int i = 0; i < neighbor_nodes.size(); ++i){

        	float temp_g_score = current_node->g + distance_between(current_node->x, current_node->y, neighbor_nodes[i]->x, neighbor_nodes[i]->y);

        	bool can_move_to = ground->canPath(neighbor_nodes[i]->x, neighbor_nodes[i]->y);
        	bool is_not_visited = ! nodeIsInVector(visited_nodes, neighbor_nodes[i]);
        	bool is_not_in_frontier = ! nodeIsInQueue(frontier_nodes, neighbor_nodes[i]);

        	if(is_not_visited && can_move_to && (is_not_in_frontier || temp_g_score < neighbor_nodes[i]->g)){
        		parent_of[neighbor_nodes[i]] = current_node;
        		neighbor_nodes[i]->g = temp_g_score;

        		if(is_not_in_frontier){
        			// printf("Trying to push node(%d, %d) into the frontier\n", neighbor_nodes[i]->x, neighbor_nodes[i]->y);
        			frontier_nodes.push(neighbor_nodes[i]);
        		}
        	}
        }
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
		temp.insert(temp.begin(), glm::vec3(origin->x, 0.0f, origin->y));
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

bool PathFinder::nodeIsInQueue(std::priority_queue<Node*, std::vector<Node*>, LessThanByGScore> list, Node* node){
	while(! list.empty()){
		Node* temp = list.top();
		list.pop();

		if((node->x == temp->x) && (node->y == temp->y)){
			return true;
		}
	}
	return false;
}

bool PathFinder::nodeIsInVector(std::vector<Node*> list, Node* node){
	for(int i = 0; i < list.size(); ++i){
		if((node->x == list[i]->x) && (node->y == list[i]->y)){
			return true;
		}
	}
	return false;
}
