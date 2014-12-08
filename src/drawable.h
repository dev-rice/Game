#ifndef Drawable_h
#define Drawable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "texture_set.h"

class Drawable {
public:
    Drawable () {;}
    Drawable(Mesh*, GLuint);
    Drawable(Mesh*, GLuint, glm::vec3, GLfloat);
    Drawable(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);

    void draw(glm::mat4*, glm::mat4*, Light*);
    void moveTo(glm::vec3);
    void setRotation(glm::vec3);
    void setScale(GLfloat);

    glm::vec3 getPosition();

    void attachTextureSet(TextureSet);


private:

    void load(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);
    Mesh* mesh;

    GLuint shader_program;

    GLfloat scale;

    glm::vec3 position;
    glm::vec3 rotation;
    
    glm::mat4 model_matrix;

    TextureSet texture_set;
};

#endif