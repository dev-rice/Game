#ifndef World_h
#define World_h

#include <vector>
#include <random>

#include "mesh.h"
#include "drawable.h"
#include "camera.h"
#include "texture_container.h"

class World{
public:
    World(GLFWwindow*, TextureContainer*);

    void update();
    void handleInputs();

private:
    Camera camera;

    std::vector<Mesh> meshes;
    std::vector<Drawable> drawables;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;

    GLFWwindow* window;
    TextureContainer* textures;
};

#endif