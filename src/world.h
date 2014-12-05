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
#include "level.h"

class World{
public:
    World(GLFWwindow*);

    void update();
    void handleInputs();

private:

    Level level;
};

#endif