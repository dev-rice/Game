#include "world.h"

World::World(GLFWwindow* window){
    this->window = window;
    addLevel("res/maps/heightmaptest.map");
}

World::World(GLFWwindow* window, const char* level_filename){
    this->window = window;
    addLevel(level_filename);
}

World::~World(){

}

void World::addLevel(const char* filename){
    Debug::info("Setting the world level to '%s'\n", filename);
    level = new Level(window, filename);
    game_view = new GameView(window, level);
}

void World::update(){
    game_view->update();
}
