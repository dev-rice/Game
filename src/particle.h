// particle.h
// Trevor Westphal

#include <glm/glm.hpp>

#include "drawable.h"
#include "mesh.h" 

class Particle{
public:
    Particle(glm::vec3, glm::vec3, float, float, int, Mesh*);
    void setInitialValues(glm::vec3, glm::vec3, float);
    void draw(glm::mat4*, glm::mat4*, Light*);
private:  
    glm::vec3 position;
    glm::vec3 initDir;

    Drawable particleDrawable;

    float initialSpeed;
    float deceleration;
    int decayTicks;
    int currentTick;
};