#include "world.h"

World::World(GLFWwindow* window){
    this->window = window;
    addLevel("res/maps/axefencefire.map");
}

World::~World(){

}

void World::addLevel(const char* filename){
    level = new Level(window, filename);
    game_view = new GameView(window, level);
}

void World::update(){
    game_view->update();
}
