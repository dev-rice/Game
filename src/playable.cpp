// playable.cpp
// Trevor Westphal

#include "playable.h"

Doodad* Playable::selection_ring;

// Magical numbers
#define AT_TARGET_PATH_RADIUS 0.1f
#define AT_TARGET_LAST_RADIUS 4.0f

#define ANGLE_PRECISION 0.1f

#define TURN_CW 1
#define TURN_CCW -1
#define TURN_NONE 0

//##################################################################################################
// Setup
//##################################################################################################
Playable::Playable() : Drawable(){

}

Playable::Playable(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale) : Drawable(mesh, shader_program, position, scale) {

    if(! selection_ring){
    	Mesh* selection_ring_mesh = new Mesh("res/models/selection_ring.dae");
    	selection_ring = new Doodad(selection_ring_mesh, shader_program, position, 2.0f);
    	selection_ring->setEmissive(TextureLoader::loadTextureFromFile("res/textures/selection_ring.png", GL_LINEAR));
        selection_ring->rotateGlobalEuler(M_PI/2.0f, 0.0f, 0.0f);
    }

	selected = false;
    temp_selected = 0;

    #warning Fix the 90* offset bug
    rotateGlobalEuler(M_PI/2.0f, 0.0f, 0.0f);

    target_position = position;
}

void Playable::loadFromXML(std::string filepath){
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());

    if(!result){
        Debug::error("Could not load unit: %s\n", filepath.c_str());
        return;
    }

    Debug::info("Parsed unit: %s\n", filepath.c_str());

    pugi::xml_node unit_node = doc.child("unit");

    unit_type = unit_node.child_value("unit_type");
    speed = std::stof(unit_node.child_value("speed"));
    acceleration = std::stof(unit_node.child_value("acceleration"));
    turning_speed = std::stof(unit_node.child_value("turning_speed"));
    radius = std::stof(unit_node.child_value("radius"));
    sight_radius = std::stof(unit_node.child_value("sight_radius"));

}

void Playable::updateUniformData(){
	glUniform1f(glGetUniformLocation(shader_program, "scale"), scale);
}

//##################################################################################################
// Order Receiving, Order Helpers, and Simple Orders (Stop, Hold)
//##################################################################################################
void Playable::receiveOrder(Playable::Order order, glm::vec3 target, bool should_enqueue, std::vector<glm::vec3> path, Playable* targeted_unit){
    
    bool is_targeting = bool(targeted_unit);

    Playable::Order body_order = determineBodyOrder(order, is_targeting);
    Playable::Order last_order = determineLastOrder(order, is_targeting);

    std::vector<std::tuple<Playable::Order, glm::vec3>> temp_order_queue;

    for(int i = 0; i < path.size(); ++i){
        temp_order_queue.insert(temp_order_queue.begin(), std::make_tuple(body_order, path[i]));
    }

    temp_order_queue.insert(temp_order_queue.begin(), std::make_tuple(last_order, target));

    if(should_enqueue){

        order_queue.insert(order_queue.end(), temp_order_queue.begin(), temp_order_queue.end());

    } else {
        // Out with the old
        targeted_units.clear();
        order_queue.clear();

        // In with the new
        target_position = position;
        order_queue = temp_order_queue;        
    }

    if(is_targeting){
        targeted_units.insert(targeted_units.begin(), targeted_unit);
    }

    // First order
    target_order = std::get<0>(order_queue.back());
    setTargetPositionAndDirection(std::get<1>(order_queue.back()));
    setRotationEuler(rotation.x, target_direction, rotation.z);
    order_queue.pop_back();
}

Playable::Order Playable::determineBodyOrder(Playable::Order order, bool is_targeting){
    if(order == Playable::Order::MOVE){
        return Playable::Order::MOVE;
    } else if (order == Playable::Order::ATTACK && is_targeting){
        return Playable::Order::MOVE;
    } else {
        return Playable::Order::ATTACK_MOVE;
    }
}

Playable::Order Playable::determineLastOrder(Playable::Order order, bool is_targeting){

    if(order == Playable::Order::STOP || order == Playable::Order::HOLD_POSITION){
        return order;
    }

    if(order == Playable::Order::MOVE && is_targeting){
        return Playable::Order::MOVE_TARGET;
    } else if (order == Playable::Order::MOVE){
        return Playable::Order::MOVE;
    } else if (order == Playable::Order::ATTACK && is_targeting){
        return Playable::Order::ATTACK_TARGET;
    } else {
        return Playable::Order::ATTACK_MOVE;
    }
}

void Playable::holdPosition(){
    // Nothing yet
}

void Playable::stop(){
    // Nothing yet
}

//##################################################################################################
// Location Helper and Maintenance functions
//##################################################################################################
bool Playable::atTargetPosition(){
    if(order_queue.size() == 0){
        // We're at the last position
        return getDistance(position.x, position.z, target_position.x, target_position.z) < AT_TARGET_PATH_RADIUS;
    } else {
        return getDistance(position.x, position.z, target_position.x, target_position.z) < AT_TARGET_LAST_RADIUS;
    }
}

float Playable::getDistance(float a1, float a2, float b1, float b2){
    float x_diff = fabs(a1 - b1);
    float z_diff = fabs(a2 - b2);
    return sqrt(x_diff*x_diff + z_diff*z_diff);
}

void Playable::setTargetPositionAndDirection(glm::vec3 target){
    old_target_position = target_position;
    target_position = target;

    target_direction = getCurrentTargetDirection();
}

float Playable::getCurrentTargetDirection(){
    float x_delta = target_position.x - position.x;
    float z_delta = target_position.z - position.z;
    return atan2(x_delta, z_delta);
}

//##################################################################################################
// Steering and Helper Functions
//##################################################################################################
int Playable::steerToStayOnPath(){
    // -1 means CCW
    //  0 means none
    //  1 means CW

    // Short-circuit for the last target, we want to move precisely to it.
    if(order_queue.size() == 0){

        // Set the target position and (more importantly) the current target direction
        float current_target_direction = getCurrentTargetDirection();

        // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles
        float angle_delta = atan2(sin(current_target_direction-rotation.y), cos(current_target_direction-rotation.y));

        // Needs to rotate to face the movement direction
        if(fabs(angle_delta) > ANGLE_PRECISION){
            if(angle_delta < 0){
                return TURN_CCW;
            }
            return TURN_CW;
        }
        return TURN_NONE;
    }

    // Predict future point           * Scaling the amount of prediction would go here
    float prediction_x = position.x + sin(rotation.y);
    float prediction_z = position.z + cos(rotation.y);

    // Put them in vec2s 
    glm::vec2 line_0 = glm::vec2(old_target_position.x, old_target_position.z);
    glm::vec2 line_1 = glm::vec2(target_position.x, target_position.z);
    glm::vec2 point = glm::vec2(prediction_x, prediction_z);

    // Find the point on the path nearest to prediction
    float distance = distanceFromPointToLine(line_0, line_1, point);

    // Steer the appropriate direction
    if(distance > 2.0){
        glm::vec2 result_steering_CCW = glm::vec2(position.x + sin(rotation.y - turning_speed), 
                                                  position.z + cos(rotation.y - turning_speed));

        glm::vec2 result_steering_CW  = glm::vec2(position.x + sin(rotation.y + turning_speed),
                                                  position.z + cos(rotation.y + turning_speed));

        float CCW_distance = distanceFromPointToLine(line_0, line_1, result_steering_CCW);
        float CW_distance  = distanceFromPointToLine(line_0, line_1, result_steering_CW);

        if(CCW_distance > CW_distance){
            return TURN_CW;
        } else {
            return TURN_CCW;
        }
    }

    return TURN_NONE;
}

float Playable::distanceFromPointToLine(glm::vec2 line_0, glm::vec2 line_1, glm::vec2 point){
     glm::vec2 v = line_1 - line_0;
     glm::vec2 w = point - line_0;

     float c1 = glm::dot(w,v);
     float c2 = glm::dot(v,v);
     float b = c1 / c2;

     glm::vec2 Pb = line_0 + (b * v);
     return getDistance(point.x, point.y, Pb.x, Pb.y);
}

//##################################################################################################
// The Update Function
//##################################################################################################
void Playable::update(Terrain* ground, std::vector<Playable*> *otherUnits){

    // Scan all units, looking for a stuff
    // Nearest friendly, hurt unit - for healing
    // Nearest friendly town hall  - resource return
    // Nearest Enemy unit/struct   - to attack if in engage range
    // Nearest resource            - to gather

    if(atTargetPosition() && order_queue.size() > 0){

        // Get the next order and target from the queue
        target_order = std::get<0>(order_queue.back());
        setTargetPositionAndDirection(std::get<1>(order_queue.back()));
        order_queue.pop_back();

    } else if(!atTargetPosition()){

        int path_steer = steerToStayOnPath();

        position.x += sin(rotation.y + (turning_speed * path_steer))*speed;
        position.z += cos(rotation.y + (turning_speed * path_steer))*speed;

        setRotationEuler(rotation.x, rotation.y + (turning_speed * path_steer), rotation.z);

    } else {

        // Do nothing... Attack nearby enemies, randomly turn and idle animate

    }

    position.y = ground->getHeightInterpolated(position.x, position.z);







































    // bool can_move = true;

    // float move_to_x = position.x;
    // float move_to_z = position.z;

    // // If THIS is not at it's target position
    // if(isMoving()){

    //     // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles
    //     float angle_delta = atan2(sin(movement_target_direction-rotation.y), cos(movement_target_direction-rotation.y));

    //     // Needs to rotate to face the movement direction
    //     if(fabs(angle_delta) > 0.01){
    //         float sign = 1.0f;

    //         if(angle_delta < 0){
    //             sign = -1.0f;
    //         }

    //         if(fabs(angle_delta) < turning_speed){
    //             setRotationEuler(rotation.x, movement_target_direction, rotation.z);
    //         } else {
    //             setRotationEuler(rotation.x, rotation.y + (turning_speed*sign), rotation.z);
    //         }
    //         // return; // UNCOMMENT ME IF YOU WANT TURN THAN MOVE
    //     }

    //     // Calculate where THIS intends to move
    //     move_to_x = position.x + sin(movement_target_direction)*speed;
    //     move_to_z = position.z + cos(movement_target_direction)*speed;
 

    //     // Push around or attack the other units
    //     // Pretty much bully everyone
    //     for(int i = 0; i < otherUnits.size(); ++i){

    //         // Find the x and z difference between THIS and other unit
    //         float x_delta = otherUnits[i]->getPosition().x - move_to_x;
    //         float z_delta = otherUnits[i]->getPosition().z - move_to_z;

    //         float abs_x_delta = abs(x_delta);
    //         float abs_z_delta = abs(z_delta);

    //         float distance_to_unit_after_move = sqrt(abs_x_delta*abs_x_delta + abs_z_delta*abs_z_delta);

    //         // If it's not THIS and if THIS moves too close
    //         if(otherUnits[i] != this && distance_to_unit_after_move < otherUnits[i]->getRadius() + radius){
    //             // Push the other unit

    //             // Get the push direction
    //             float theta = atan2(x_delta, z_delta);

    //             // Saving the radius
    //             float other_radius = otherUnits[i]->getRadius();

    //             // Add the distance to push to the location-to-be of THIS
    //             float push_to_x = move_to_x + sin(theta)*(other_radius + radius);
    //             float push_to_z = move_to_z + cos(theta)*(other_radius + radius);

    //             // Apply the movement to the other unit IF they aren't moving
    //             if(otherUnits[i]->canBePushed()){

    //                 bool did_push = otherUnits[i]->requestPush(ground, glm::vec3(push_to_x, 0.0f, push_to_z));

    //                 can_move &= did_push;
    //             }
    //         }
    //     }
    // }
    
    // if(can_move && isMoving()){
    //     position.x = move_to_x;
    //     position.z = move_to_z;
    // }

    // // We've arrived, but we still have internal orders
    // if( !isMoving() && internal_order_queue.size() > 0){

    //     setMovementTarget(internal_order_queue.back());
    //     internal_order_queue.pop_back();

    // }

    // // We've arrived, and we have a new order from the user
    // if( !isMoving() && order_queue.size() > 0 && internal_order_queue.size () == 0){

    //     executeOrder(std::get<0>(order_queue.back()), std::get<1>(order_queue.back()));
    //     internal_order_queue.clear();
    //     order_queue.pop_back();

    // }

}

void Playable::draw(){
    // Make the base drawable call before
    // Drawing things specific to playable
    Drawable::draw();

    if(selected || temp_selected ){
     //    selection_ring->setPosition(glm::vec3(move_to_position.x, position.y + 0.5, move_to_position.z));
    	// selection_ring->draw();

        selection_ring->setPosition(glm::vec3(position.x, position.y + 0.5, position.z));
        selection_ring->draw();
    }
}

void Playable::select(){
    selected = true;
}

void Playable::deSelect(){
    selected = false;
    temp_selected = false;
}

void Playable::tempSelect(){
    temp_selected = true;
}

void Playable::tempDeSelect(){
    temp_selected = false;
}
