// pathfinder.cpp
// Trevor Westphal

#include "pathfinder.h"

int** PathFinder::node_state_array;
int PathFinder::depth;
int PathFinder::width;

const int PathFinder::UNVISITED;
const int PathFinder::IN_FRONTIER;
const int PathFinder::VISITED;

void PathFinder::allocateArray(Terrain* ground){
	depth = ground->getDepth();
	width = ground->getWidth();

	node_state_array = new int*[width];
    for(int i = 0; i < width; ++i){
        node_state_array[i] = new int[depth];
        for(int j = 0; j < depth; ++j){
        	node_state_array[i][j] = UNVISITED;
        }
    }
}

std::vector<glm::vec3> PathFinder::find_path(Terrain *ground, int start_x, int start_y, int target_x, int target_y){

	bool can_path_line = canPathOnLine(ground, start_x, start_y, target_x, target_y);

	int x_offset = width/2;
	int y_offset = depth/2;

	std::vector<Node*> visited_nodes;											    // The set of nodes already evaluated.
	
	std::priority_queue<Node*, std::vector<Node*>, LessThanByGScore> frontier_nodes;// The set of tentative nodes to be evaluated...
			
	Node *start_node = new Node(start_x, start_y, 0.0f);							// ...initially containing the start node		
	frontier_nodes.push(start_node);												// Cost from start along best known path (included)							                
												
	std::map<Node*, Node*> parent_of;												// The map of navigated nodes.
																					
	int count = 0;
	int index_x, index_y;
	float temp_g_score;
	float distance_to_goal;
	bool can_move_to;
	bool is_not_visited;
	bool is_not_in_frontier;
	int node_state;

	Node* closest_node;
	float closest_node_distance = 99999.0f;

 	while(! frontier_nodes.empty()){
 		count ++;
 		Node* current_node = frontier_nodes.top();

        if(current_node->x == target_x && current_node->y == target_y){
        	// Clear out the old visited nodes
        	while(! frontier_nodes.empty()){
        		node_state_array[frontier_nodes.top()->x + x_offset][frontier_nodes.top()->y + y_offset] = UNVISITED;
        		frontier_nodes.pop();
        	}
        	for(int i = 0; i < visited_nodes.size(); ++i){
        		node_state_array[visited_nodes[i]->x + x_offset][visited_nodes[i]->y + y_offset] = UNVISITED;
        	}
        	return reconstruct_path(ground, parent_of, current_node);
        }

		distance_to_goal = distance_between(current_node->x, current_node->y, target_x, target_y);
        if(distance_to_goal < closest_node_distance){
        	closest_node_distance = distance_to_goal;
        	closest_node = current_node;
        }

        frontier_nodes.pop();

        visited_nodes.push_back(current_node);
        node_state_array[current_node->x + x_offset][current_node->y + y_offset] = VISITED;

        std::vector<Node*> neighbor_nodes = getNeighborNodes(current_node);

        for(int i = 0; i < neighbor_nodes.size(); ++i){

        	Node* n = neighbor_nodes[i];
        	index_x = n->x + x_offset;
        	index_y = n->y + y_offset;

        	temp_g_score = current_node->g + distance_between(current_node->x, current_node->y, n->x, n->y);

        	can_move_to = ground->canPath(n->x, n->y);
        	can_move_to &= ground->canPath(n->x + 1, n->y);
        	can_move_to &= ground->canPath(n->x - 1, n->y);
        	can_move_to &= ground->canPath(n->x,     n->y + 1);
        	can_move_to &= ground->canPath(n->x,     n->y - 1);

        	is_not_visited = false;
        	is_not_in_frontier = false;

        	if(index_y > 0 && index_y < depth && index_x > 0 && index_x < width){
        		node_state = node_state_array[n->x + x_offset][n->y + y_offset];
        		is_not_visited = node_state == UNVISITED;
        		is_not_in_frontier = node_state != IN_FRONTIER;
        	}

        	if(is_not_visited && can_move_to && (is_not_in_frontier || temp_g_score < n->g)){
        		parent_of[n] = current_node;
        		n->g = temp_g_score;

        		if(is_not_in_frontier){
        			frontier_nodes.push(n);
    				node_state_array[n->x + x_offset][n->y + y_offset] = IN_FRONTIER;
        		}
        	}
        }
 	}

 	// Clear out the old visited nodes
	for(int i = 0; i < visited_nodes.size(); ++i){
		node_state_array[visited_nodes[i]->x + x_offset][visited_nodes[i]->y + y_offset] = 0;
	}
	return reconstruct_path(ground, parent_of, closest_node);
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

std::vector<glm::vec3> PathFinder::reconstruct_path(Terrain *ground, std::map<Node*, Node*> parent_of, Node* origin){
	std::vector<glm::vec3> temp;

	if(! origin){
		return temp;
	}

	temp.push_back(glm::vec3(origin->x, 0.0f, origin->y));
	while(parent_of[origin]){
		origin = parent_of[origin];
		temp.insert(temp.begin(), glm::vec3(origin->x, 0.0f, origin->y));
	}

	// Smoothing goes here

	return temp;
}

std::vector<Node*> PathFinder::getNeighborNodes(Node *current_node){
	std::vector<Node*> temp;
	// could do up/down/right/left only
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

bool PathFinder::canPathOnLine(Terrain* ground, int x0, int y0, int x1, int y1){
	// http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_for_integer_arithmetic
	// dx=x1-x0
	// dy=y1-y0

	// D = 2*dy - dx
	// plot(x0,y0)
	// y=y0

	// for x from x0+1 to x1
	// if D > 0
	//   y = y+1
	//   plot(x,y)
	//   D = D + (2*dy-2*dx)
	// else
	//   plot(x,y)
	//   D = D + (2*dy)

	// ONLY WORKS FOR OCTANT 0:
	//  \5|6/
	//  4\|/7
	// ---+---
	//  3/|\0
	//  /2|1\

	printf("Starting line draw...\n");
	int dx = x1 - x0;
	int dy = y1 - y0;

	int d = (dy + dy) - dx;
	printf("(%d, %d)\n", x0, y0);
	int y = y0;

	for(int x = x0 + 1; x < x1; ++x){
		if(d > 0){
			y++;
			printf("(%d, %d)\n", x, y);
			d += (dy + dy) - (dx + dx);
		} else {
			printf("(%d, %d)\n", x, y);
			d += (dy + dy);
		}
	}

	printf("(%d, %d)\n", x1, y1);
	printf("Ended line draw\n\n");


	return true;
}
