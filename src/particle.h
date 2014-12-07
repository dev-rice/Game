// particle.h
// Trevor Westphal

#include <glm/glm.hpp>

class Particle{
public:
    Particle(glm::vec3, glm::vec3, float, float, int);
    void setInitialValues(glm::vec3, glm::vec3, float);
    void draw();
private:  
    glm::vec3 position;
    glm::vec3 initDir;

    float initialSpeed;
    float deceleration;
    int decayTicks;
};