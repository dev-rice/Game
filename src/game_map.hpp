#ifndef GameMap_h
#define GameMap_h

#include <vector>
#include <functional>
#include <fstream>

#include "doodad.h"
#include "particles/all_emitters.hpp"
#include "terrain.h"
#include "includes/json.hpp"

using namespace std;

class GameMap {
public:
    GameMap(string map_filename);

    void render();


private:

    void load(ifstream& map);

    Camera camera;
    vector<reference_wrapper<Doodad>> doodads;
    vector<reference_wrapper<Emitter>> emitters;
    Terrain terrain;

};

#endif
