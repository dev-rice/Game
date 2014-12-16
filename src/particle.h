// particle.h
// Trevor Westphal

#ifndef Particle_h
#define Particle_h

#include <glm/glm.hpp>
#include <math.h>

#include "drawable.h"
#include "camera.h"
#include "mesh.h" 

class Particle : public Drawable {
public:
    enum class ScalingOption { SCALE_UP_WITH_AGE, SCALE_SMOKE, SCALE_DOWN_WITH_AGE, SCALE_NONE};
    enum class FadingOption { FADE_OUT_WITH_AGE, FADE_IN_WITH_AGE, FADE_SMOKE, FADE_NONE};

    Particle(Mesh*, GLuint);
    Particle(Mesh*, GLuint, glm::vec3, GLfloat);
    Particle(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);

    // ~Particle();
    
    void setInitialValues(glm::vec3, glm::vec3, glm::vec3, float, int, ScalingOption, FadingOption);
    void draw(Camera*, glm::mat4*);
    bool isDead();
    void enablePhysics(float);
    virtual void attachTextureSet(TextureSet);

private:  
    void updateUniformData();
    void bindTextures();

    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 dir;

    float rotationSpeed;
    float plane_rotation;
    float elasticity;
    float opacity;

    bool physicsEnabled;

    ScalingOption scaleWithAge;
    FadingOption alphaWithAge;

    int lifespan;
    int age;

};
#endif