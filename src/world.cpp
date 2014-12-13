#include "world.h"

World::World(GLFWwindow* window){
    this->window = window;

    // addLevel("res/maps/scene.map");
    // addLevel("res/maps/axefence.map");
    addLevel("res/maps/axefencefire.map");

}

World::~World(){

}

void World::addLevel(const char* filename){
    Level* level = new Level(window, filename);
    levels.push_back(level);
    
    level = NULL;
    delete level;

    // For now the current level is set to whatever was just added.
    current_level = levels.size() - 1;
}

void World::update(){
    levels[current_level]->handleInputs();
    levels[current_level]->draw();
}