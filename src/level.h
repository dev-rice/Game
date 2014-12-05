// level_loader.h
// Trevor Westphal

#ifndef Level_h
#define Level_h

#include <vector>

#include "drawable.h"
#include "mesh.h"
#include "texture_container.h"
#include "camera.h"
#include "shader_loader.h"
#include "light.h"

static const char* MODEL_PATH = "res/models/";
static const char* TEXTURE_PATH = "res/textures/";


class Level{
public:
    Level() {;}
    Level(GLFWwindow*);
    void loadLevel(const char *);

    void draw();

    void handleInputs();

    std::vector<Drawable*> getDrawable();
private:
    GLFWwindow* window;
    
    Camera camera;
    Light light;

    TextureContainer textures;
    std::vector<Mesh> meshes;
    std::vector<Drawable> drawables;

    GLuint shader_program;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;

};

#endif