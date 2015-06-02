#ifndef Drawable_h
#define Drawable_h

#include "includes/gl.hpp"
#include "includes/glm.hpp"

#include <cmath>

#include "mesh.h"
#include "camera.h"

#include "shader_loader.h"
#include "texture_loader.h"

class Drawable {
public:
    Drawable () {;}
    Drawable(Mesh*, Shader shader);
    Drawable(Mesh*, Shader shader, glm::vec3, GLfloat);

    virtual void draw();

    void setRotationEuler(GLfloat x, GLfloat y, GLfloat z);
    void setRotationEuler(glm::vec3 rotation);

    void rotateGlobalEuler(GLfloat x, GLfloat y, GLfloat z);
    void rotateGlobalEuler(glm::vec3 rotation);

    void rotateAxisAngle(glm::vec3 axis, GLfloat angle);

    void setPosition(glm::vec3 p);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);

    void setScale(GLfloat s) {scale = s;}
    void setShader(Shader shader);

    void setDiffuse(GLuint d);
    void setSpecular(GLuint s);
    void setEmissive(GLuint e);
    void setNormal(GLuint n);

    glm::vec3 getPosition() {return position;}
    GLfloat getScale() {return scale;}
    Shader& getShader() {return shader;}

protected:
    void load(Mesh*, Shader shader, glm::vec3, GLfloat);
    void updateModelMatrix();

    virtual void bindTextures();
    virtual void setTextureLocations();
    virtual void updateUniformData() = 0;

    Mesh* mesh;

    Shader shader;

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
