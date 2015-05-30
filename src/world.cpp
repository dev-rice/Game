#include "world.h"

World::World(string level_filename, bool edit_mode){
    this->edit_mode = edit_mode;
    addLevel(level_filename);
}

World::~World(){

}

void World::addLevel(string filename){
    Debug::info("Setting the world level to '%s'\n", filename.c_str());
    GameMap map(filename);
    if (edit_mode){
        game_view = new GameViewEdit(map);
    } else {
        game_view = new GameView(map);
    }
}

void World::update(){
    game_view->update();
}
