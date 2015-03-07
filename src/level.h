// level_loader.h
// Trevor Westphal

#ifndef Level_h
#define Level_h

#include <vector>
#include <string>
#include <cmath>
#include <math.h>       // sqrt
#include <algorithm>    // std::max
#include <cfloat>       // FLT_MAX
#include <random>

#include "drawable.h"
#include "doodad.h"
#include "mesh.h"
#include "camera.h"
#include "shader_loader.h"
#include "texture_loader.h"
#include "playable.h"
#include "pathfinder.h"

#include "terrain.h"

#include "particles/emitter.h"
#include "particles/fire_emitter.h"
#include "particles/snow_emitter.h"
#include "particles/smoke_emitter.h"

#include "shadowbuffer.h"
#include "window.h"
#include "profile.h"
#include "game_clock.h"

static const char* MODEL_PATH = "res/models/";
static const char* TEXTURE_PATH = "res/textures/";
static const char* DEFAULT_MAP = "res/maps/default.map";

class Level{
public:
    ~Level();
    Level(const char*);

    void draw();
    void drawShadowMap();
    void updateGlobalUniforms();

    int getMapDepth();
    int getMapWidth();
    Terrain* getTerrain() {return ground;}
    glm::mat4 getProjection() {return proj_matrix;}

    // Need to add type of order
    void issueOrder(Playable::Order, glm::vec3, bool);
    void selectUnit(glm::vec3);
    void selectUnits(glm::vec3, glm::vec3);
    void tempSelectUnits(glm::vec3, glm::vec3);

    float getDistance(float, float, float, float);

    glm::vec3 getIntersection(glm::vec3 line, float plane_height);
    glm::vec3 calculateRay(glm::vec2 screen_point);
    std::tuple<float, float, glm::vec3> findWorldPoint(glm::vec3 ray, int steps, float bottom, float top);
    glm::vec3 findWorldPointInit(glm::vec3 ray, int steps);
    glm::vec3 calculateWorldPosition(glm::vec2 screen_point);

    Camera* getCamera() {return camera;}
    Shadowbuffer* getShadowbuffer(){return shadowbuffer;}

private:
    void loadLevel(const char *);
    GLuint getTexture(GLuint);

    Window* window;
    Camera* camera;

    Terrain *ground;

    std::vector<GLuint> textures;
    std::vector<Mesh*> meshes;
    std::vector<Drawable*> drawables;
    std::vector<Drawable*> non_shadow_drawables;
    std::vector<Playable*> selected_units;
    std::vector<Playable*> units;
    std::vector<Emitter*> emitters;

    GLuint doodad_shader;
    GLuint terrain_shader;
    GLuint particle_shader;
    GLuint shadow_shader;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;

    glm::mat4 depth_view;
    glm::mat4 depth_proj;

    Shadowbuffer* shadowbuffer;

    GLuint camera_ubo;
    GLuint shadow_ubo;

};

#endif
