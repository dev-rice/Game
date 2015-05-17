#include "world.h"

World::World(bool edit_mode) : World(DEFAULT_MAP, edit_mode){}

World::World(const char* level_filename, bool edit_mode){
    this->window = Window::getInstance();
    this->edit_mode = edit_mode;
    addLevel(level_filename);
}

World::~World(){

}

void World::addLevel(const char* filename){
    Debug::info("Setting the world level to '%s'\n", filename);
    level = new Level(filename);
    GameMap map("res/maps/newformat.map");
    if (edit_mode){
        game_view = new GameViewEdit(level, map);
    } else {
        game_view = new GameView(level, map);
    }
}

void World::update(){
    game_view->update();
}
