#include "world.h"

World::World(GLFWwindow* window){
    level = Level(window);
    level.loadLevel("res/maps/scene.map");

}

void World::update(){
    level.handleInputs();
    level.draw();
}