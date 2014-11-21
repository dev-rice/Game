#ifndef World_h
#define World_h

#include <vector>

#include "mesh.h"
#include "drawable.h"
#include "camera.h"

class World{
public:
    World(float, float);

    void update();
    void handleInputs(GLFWwindow*);

private:
    Camera camera;

    std::vector<Mesh> meshes;
    std::vector<Drawable> drawables;

    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;
};

#endif