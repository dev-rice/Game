#ifndef DrawablePlacer_h
#define DrawablePlacer_h

#include "includes/glm.hpp"
#include "includes/sdl.hpp"

#include "drawable.h"
#include "resource_loader.hpp"
#include "doodad.h"
#include "level.hpp"
#include "drawable_selector.hpp"

class DrawablePlacer {
public:
    DrawablePlacer(Level& level);

    void setDrawable(Drawable& drawable);
    Drawable& getDrawable();

    void update(glm::vec3 mouse_world_pos);
    void handleInput(SDL_Event event);

    void activate();
    void deactivate();

private:

    Drawable* createDefaultDoodad();

    DrawableSelector selector;
    Level* level;
    Drawable* current_drawable;
    glm::vec3 axis;

};

#endif
