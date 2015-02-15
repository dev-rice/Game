#include "world.h"

World::World() : World("res/maps/default.map"){}

World::World(const char* level_filename){
    this->window = Window::getInstance();
    addLevel(level_filename);
}

World::~World(){

}

void World::addLevel(const char* filename){
    Debug::info("Setting the world level to '%s'\n", filename);
    level = new Level(filename);
    game_view = new GameView(level);
}

void World::update(){
    game_view->update();
}
