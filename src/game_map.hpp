#ifndef GameMap_h
#define GameMap_h

#include <vector>
#include <functional>
#include <fstream>
#include <iostream>

#include "doodad.h"
#include "terrain.h"
#include "includes/json.hpp"
#include "shadowbuffer.h"

#include "render_stack.hpp"
#include "profile.h"
#include "particles/emitter_factory.hpp"
#include "unit_holder.hpp"

using namespace std;

class GameMap {
public:
    GameMap(string map_filename, UnitHolder& unit_holder, RenderStack& render_stack);

    void render();
    void renderToShadowMap();
    void renderToDepthMap();

    void addDrawable(Drawable& drawable);

    glm::vec3 calculateWorldPosition(glm::vec2 screen_pos);

    Shadowbuffer& getShadowbuffer();
    Shadowbuffer& getDepthbuffer();

    Camera& getCamera();
    Terrain& getGround();

private:

    void load(ifstream& map);

    void initializeGlobalUniforms();
    void updateGlobalUniforms();

    glm::vec3 getIntersection(glm::vec3 line, float plane_height);
    glm::vec3 calculateRay(glm::vec2 screen_point);
    std::tuple<float, float, glm::vec3> findMapPoint(glm::vec3 ray, int steps, float bottom, float top);
    glm::vec3 findMapPointInit(glm::vec3 ray, int steps);

    Camera camera;
    vector<Doodad> doodads;
    vector<Emitter*> emitters;
    Terrain ground;
    UnitHolder* unit_holder;
    RenderStack* render_stack;

    // Everything that will be drawn
    // vector<Drawable> drawables;

    // // Anything that shouldn't be drawn to the shadow map
    // vector<Drawable> no_shadow_drawables;

    Shadowbuffer shadowbuffer;
    Shadowbuffer depthbuffer;

    GLuint camera_ubo;
    GLuint shadow_ubo;

    Shader shadow_shader;
    Shader depth_shader;

};

#endif
