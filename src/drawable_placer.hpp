#ifndef DrawablePlacer_h
#define DrawablePlacer_h

#include "includes/glm.hpp"
#include "drawable.h"
#include "resource_loader.hpp"
#include "doodad.h"

class DrawablePlacer {
public:
    DrawablePlacer();

    void setDrawable(Drawable& drawable);

    void placeCurrentDrawable();
    void update(glm::vec3 mouse_world_pos);

    Drawable& getDrawable();

private:
    Drawable* current_drawable;
    glm::vec3 axis;

    // This is only a temporary setback
    ResourceLoader resource_loader;
};

#endif
