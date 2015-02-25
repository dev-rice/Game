#ifndef Drawable_h
#define Drawable_h

#include <GL/glew.h>

#if defined __APPLE__ && __MACH__
    #include <OpenGL/OpenGL.h>
// #elif defined __gnu_linux__
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>

#include "mesh.h"
#include "camera.h"

#include "shader_loader.h"
#include "texture_loader.h"

class Drawable {
public:
    Drawable () {;}
    Drawable(Mesh*, GLuint);
    Drawable(Mesh*, GLuint, glm::vec3, GLfloat);

    virtual void draw();

    void setRotationEuler(GLfloat x, GLfloat y, GLfloat z);
    void setRotationEuler(glm::vec3 rotation);


    void rotateGlobalEuler(GLfloat x, GLfloat y, GLfloat z);
    void rotateGlobalEuler(glm::vec3 rotation);

    void rotateAxisAngle(glm::vec3 axis, GLfloat angle);

    void setPosition(GLfloat x, GLfloat y, GLfloat z){ position = glm::vec3(z, y, z); }
    void setPosition(glm::vec3 p) {position = p;}
    void setScale(GLfloat s) {scale = s;}
    void setShader(GLuint);

    void setDiffuse(GLuint d);
    void setSpecular(GLuint s);
    void setEmissive(GLuint e);
    void setNormal(GLuint n);

    glm::vec3 getPosition() {return position;}
    GLfloat getScale() {return scale;}
    GLuint getShader() {return shader_program;}

protected:
    void load(Mesh*, GLuint, glm::vec3, GLfloat);
    void updateModelMatrix();

    virtual void bindTextures();
    virtual void setTextureLocations();
    virtual void updateUniformData() = 0;

    Mesh* mesh;

    GLuint shader_program;

    GLfloat scale;

    glm::vec3 position;
    glm::vec3 rotation;

    glm::mat4 model_matrix;
    glm::mat4 rotation_matrix;

    GLuint diffuse;
    GLuint specular;
    GLuint emissive;
    GLuint normal;
    GLuint gloss;
};

#endif
