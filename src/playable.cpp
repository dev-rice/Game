// playable.cpp
// Trevor Westphal

#include "playable.h"

Doodad* Playable::selection_ring;

Playable::Playable() : Drawable(){

}

Playable::Playable(Mesh* mesh, GLuint shader_program, glm::vec3 position, GLfloat scale) : Drawable(mesh, shader_program, position, scale) {

    if(! selection_ring){
    	Mesh* selection_ring_mesh = new Mesh("res/models/selection_ring.dae");
    	selection_ring = new Doodad(selection_ring_mesh, shader_program, position, 1.0f);
    	selection_ring->setEmissive(TextureLoader::loadTextureFromFile("res/textures/selection_ring.png", GL_LINEAR));
        selection_ring->rotateGlobalEuler(M_PI/2.0f, 0.0f, 0.0f);
    }

	selected = false;
    temp_selected = 0;

    #warning Fix the 90* offset bug
    rotateGlobalEuler(M_PI/2.0f, 0.0f, 0.0f);

    move_to_position = position;
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

bool Playable::receiveOrder(Playable::Order order, glm::vec3 target, bool queue){
    if(!queue){
        order_queue.clear();
        executeOrder(order, target);
        return true; // like below, todo
    }

    order_queue.insert(order_queue.begin(), std::make_tuple(order, target));

    // Later, have this return validity of order
    return true;
}

void Playable::executeOrder(Playable::Order order, glm::vec3 target){
    switch(order){
        case Playable::Order::MOVE:
            needs_pathing_on_update = true;
            setMovementTarget(target);
            break;
        case Playable::Order::ATTACK:
            needs_pathing_on_update = true;
            setMovementTarget(target); // Attack not done yet
            break; 
        case Playable::Order::HOLD_POSITION:
            holdPosition();
            break;
        case Playable::Order::STOP:
            stop();
            break;
    }
}

bool Playable::requestPush(Terrain *ground, glm::vec3 pos){
    if(holding_position){
        return false;
    }

    if(!has_been_push_requested){
        if(ground->canPath(int(pos.x), int(pos.z))){
            setMovementTarget(pos);
            return true;
        } else {
            return false;
        }        
    }
}

void Playable::holdPosition(){
    setMovementTarget(position);
    holding_position = true;
}

void Playable::stop(){
    setMovementTarget(position);
}

void Playable::setMovementTarget(glm::vec3 pos){
    holding_position = false;
    move_to_position = pos;

    float x_delta = move_to_position.x - position.x;
    float z_delta = move_to_position.z - position.z;
    movement_target_direction = atan2(x_delta, z_delta);
}

bool Playable::isMoving(){
    float x_delta = move_to_position.x - position.x;
    float z_delta = move_to_position.z - position.z;

    return (sqrt(x_delta*x_delta + z_delta*z_delta) > 0.1);
}

bool Playable::canBePushed(){
    return !isMoving() && (order_queue.size() == 0);
}

void Playable::update(Terrain* ground, std::vector<Playable*> otherUnits){

    // If the units needs pathing (having just been given a new order)
    if(needs_pathing_on_update){
        needs_pathing_on_update = false;
        internal_order_queue.clear();
        std::vector<glm::vec3> temp = PathFinder::find_path(ground, int(position.x), int(position.z), int(move_to_position.x), int(move_to_position.z));

        for(int i = 0; i < temp.size(); ++i){
            internal_order_queue.insert(internal_order_queue.begin(), temp[i]);
        }   

        if(internal_order_queue.size() > 0){
            internal_order_queue.insert(internal_order_queue.begin(), move_to_position);

            // Go to the first one
            setMovementTarget(internal_order_queue.back());
            internal_order_queue.pop_back();
        } else {
            stop();
        }
    }

    bool can_move = true;

    float move_to_x = position.x;
    float move_to_z = position.z;

    // If THIS is not at it's target position
    if(isMoving()){

        // http://stackoverflow.com/questions/1878907/the-smallest-difference-between-2-angles
        float angle_delta = atan2(sin(movement_target_direction-rotation.y), cos(movement_target_direction-rotation.y));

        // Needs to rotate to face the movement direction
        if(fabs(angle_delta) > 0.01){
            float sign = 1.0f;

            if(angle_delta < 0){
                sign = -1.0f;
            }

            if(fabs(angle_delta) < turning_speed){
                setRotationEuler(rotation.x, movement_target_direction, rotation.z);
            } else {
                setRotationEuler(rotation.x, rotation.y + (turning_speed*sign), rotation.z);
            }
            // return; // UNCOMMENT ME IF YOU WANT TURN THAN MOVE
        }

        // Calculate where THIS intends to move
        move_to_x = position.x + sin(movement_target_direction)*speed;
        move_to_z = position.z + cos(movement_target_direction)*speed;
    } 

    // Push around or attack the other units
    // Pretty much bully everyone
    for(int i = 0; i < otherUnits.size(); ++i){

        // Find the x and z difference between THIS and other unit
        float x_delta = otherUnits[i]->getPosition().x - move_to_x;
        float z_delta = otherUnits[i]->getPosition().z - move_to_z;

        float abs_x_delta = abs(x_delta);
        float abs_z_delta = abs(z_delta);

        float distance_to_unit_after_move = sqrt(abs_x_delta*abs_x_delta + abs_z_delta*abs_z_delta);

        // If it's not THIS and if THIS moves too close
        if(otherUnits[i] != this && distance_to_unit_after_move < otherUnits[i]->getRadius() + radius){
            // Push the other unit

            // Get the push direction
            float theta = atan2(x_delta, z_delta);

            // Saving the radius
            float other_radius = otherUnits[i]->getRadius();

            // Add the distance to push to the location-to-be of THIS
            float push_to_x = move_to_x + sin(theta)*(other_radius + radius);
            float push_to_z = move_to_z + cos(theta)*(other_radius + radius);

            // Apply the movement to the other unit IF they aren't moving
            if(otherUnits[i]->canBePushed()){

                bool did_push = otherUnits[i]->requestPush(ground, glm::vec3(push_to_x, 0.0f, push_to_z));

                can_move &= did_push;
            }
        }
    }

    // can_move &= ground->getSteepness(move_to_x, move_to_z) < 0.8f;
    // can_move &= ground->isOnTerrain(move_to_x, move_to_z, 1.0); 
    
    if(can_move && isMoving()){
        position.x = move_to_x;
        position.z = move_to_z;
    }

    // We've arrived, but we still have internal orders
    if( !isMoving() && internal_order_queue.size() > 0){

        setMovementTarget(internal_order_queue.back());
        internal_order_queue.pop_back();

    }

    // We've arrived, and we have a new order from the user
    if( !isMoving() && order_queue.size() > 0 && internal_order_queue.size () == 0){

        executeOrder(std::get<0>(order_queue.back()), std::get<1>(order_queue.back()));
        internal_order_queue.clear();
        order_queue.pop_back();

    }

    position.y = ground->getHeightInterpolated(position.x, position.z);
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

    has_been_push_requested = false;
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
