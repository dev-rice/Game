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
#include "camera.h"
#include "texture_set.h"
#include "shader_loader.h"

class Drawable {
public:
    Drawable () {;}
    Drawable(Mesh*, GLuint);
    Drawable(Mesh*, GLuint, glm::vec3, GLfloat);
    Drawable(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);

    void draw();

    void setPosition(glm::vec3 p) {position = p;}
    void setRotation(glm::vec3 r) {rotation = r;}
    void setScale(GLfloat s) {scale = s;}
    void setShader(GLuint);

    void setDiffuse(GLuint d);
    void setSpecular(GLuint s);
    void setEmissive(GLuint e);
    void setNormal(GLuint n);

    glm::vec3 getPosition() {return position;}
    glm::vec3 getRotation() {return rotation;}
    GLfloat getScale() {return scale;}
    GLuint getShader() {return shader_program;}

protected:
    void load(Mesh*, GLuint, glm::vec3, glm::vec3, GLfloat);
    void updateModelMatrix();

    virtual void bindTextures();
    virtual void updateUniformData() = 0;

    Mesh* mesh;

    GLuint shader_program;

    GLfloat scale;

    glm::vec3 position;
    glm::vec3 rotation;

    glm::mat4 model_matrix;

    GLuint diffuse;
    GLuint specular;
    GLuint emissive;
    GLuint normal;
    GLuint gloss;
};

#endif
