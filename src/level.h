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

#include "emitter.h"
#include "fire_emitter.h"
#include "snow_emitter.h"
#include "smoke_emitter.h"

static const char* MODEL_PATH = "res/models/";
static const char* TEXTURE_PATH = "res/textures/";


class Level{
public:
    ~Level();
    Level(GLFWwindow*, const char*);

    void draw();
    void drawShadowMap();

    Camera* getCamera() {return camera;}

private:
    void loadLevel(const char *);
    GLuint getTexture(GLuint);

    GLFWwindow* window;

    Camera* camera;

    std::vector<GLuint> textures;
    std::vector<Mesh*> meshes;
    std::vector<Drawable*> drawables;
    std::vector<Emitter*> emitters;

    GLuint doodad_shader;
    GLuint particle_shader;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;

};

#endif
