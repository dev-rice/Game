#include "world.h"

World::World(GLFWwindow* window){
    Level* level = new Level(window, "res/maps/axefence.map");
    levels.push_back(level);
    
    level = NULL;
    delete level;

    current_level = 0;
}

void World::update(){
    levels[current_level]->handleInputs();
    levels[current_level]->draw();
}