#ifndef DrawablePlacer_h
#define DrawablePlacer_h

#include "includes/glm.hpp"
#include "drawable.h"
#include "resource_loader.hpp"
#include "doodad.h"
#include "level.hpp"

class DrawablePlacer {
public:
    DrawablePlacer(Level& level);

    void setDrawable(Drawable& drawable);

    void update(glm::vec3 mouse_world_pos);

    Drawable& getDrawable();

private:
    Level* level;
    Drawable* current_drawable;
    glm::vec3 axis;

};

#endif
