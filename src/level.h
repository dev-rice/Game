// level_loader.h
// Trevor Westphal

#ifndef Level_h
#define Level_h

#include <vector>
#include <string>

#include "drawable.h"
#include "doodad.h"
#include "mesh.h"
#include "camera.h"
#include "shader_loader.h"
#include "texture_loader.h"
#include "texture_set.h"
#include "terrain.h"

#include "particles/emitter.h"
#include "particles/fire_emitter.h"
#include "particles/snow_emitter.h"
#include "particles/smoke_emitter.h"

#include "shadowbuffer.h"
#include "window.h"

#include "mouse.h"

static const char* MODEL_PATH = "res/models/";
static const char* TEXTURE_PATH = "res/textures/";


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
    std::vector<Emitter*> emitters;

    GLuint doodad_shader;
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
