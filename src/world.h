#ifndef World_h
#define World_h

#include <vector>
#include <random>
#include <math.h>

#include "mesh.h"
#include "drawable.h"
#include "camera.h"
#include "texture_container.h"
#include "texture_set.h"
#include "light.h"

class World{
public:
    World(GLFWwindow*, std::vector<Mesh*>, TextureContainer*);

    void update();
    void handleInputs();

private:
    Camera camera;

    std::vector<Mesh*> meshes;
    std::vector<Drawable> drawables;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;

    GLFWwindow* window;
    TextureContainer* textures;

    Light light;
};

#endif