// particle.h
// Trevor Westphal

#include <glm/glm.hpp>

#include "drawable.h"
#include "camera.h"
#include "mesh.h" 

class Particle{
public:
    enum class ScalingOption { SCALE_UP_WITH_AGE, SCALE_DOWN_WITH_AGE, SCALE_NONE};
    enum class FadingOption { FADE_OUR_WITH_AGE, FADE_IN_WITH_AGE, FADE_NONE};

    Particle(Mesh*, TextureSet*, GLuint);
    // ~Particle();

    void setInitialValues(glm::vec3, glm::vec3, glm::vec3, float, int, ScalingOption, FadingOption);
    void draw(Camera*, glm::mat4*, Light*);
private:  
    glm::vec3 position;
    glm::vec3 Velocity;
    glm::vec3 acceleration;

    float rotationSpeed;

    ScalingOption scaleWithAge;
    FadingOption alphaWithAge;

    int lifespan;
    int age;

    Drawable* drawable;
};