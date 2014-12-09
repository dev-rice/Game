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
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 dir;

    float rotationSpeed;
    float x_rot;

    ScalingOption scaleWithAge;
    FadingOption alphaWithAge;

    int lifespan;
    int age;

    Drawable* drawable;
};