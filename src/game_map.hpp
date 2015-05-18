#ifndef GameMap_h
#define GameMap_h

#include <vector>
#include <functional>
#include <fstream>
#include <iostream>

#include "doodad.h"
#include "particles/all_emitters.hpp"
#include "terrain.h"
#include "includes/json.hpp"
#include "shadowbuffer.h"
#include "texture_loader.h"

using namespace std;

class GameMap {
public:
    GameMap(string map_filename);

    void render();
    void renderToShadowMap();
    Camera& getCamera();
    Terrain& getGround();

private:

    void load(ifstream& map);

    void initializeGlobalUniforms();
    void updateGlobalUniforms();

    Camera camera;
    vector<Doodad> doodads;
    vector<reference_wrapper<Emitter>> emitters;
    Terrain ground;

    // Everything that will be drawn
    vector<reference_wrapper<Drawable>> drawables;

    // Anything that shouldn't be drawn to the shadow map
    vector<reference_wrapper<Drawable>> no_shadow_drawables;

    Shadowbuffer shadowbuffer;

    GLuint camera_ubo;
    GLuint shadow_ubo;

    GLuint shadow_shader;

};

#endif
