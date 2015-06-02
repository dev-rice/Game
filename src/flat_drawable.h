#ifndef FlatDrawable_h
#define FlatDrawable_h

#include "includes/gl.hpp"
#include "includes/glm.hpp"

#include "mesh.h"
#include "flat_mesh.h"
#include "shader_loader.h"

class FlatDrawable {
public:
    FlatDrawable() : FlatDrawable(1.0, 1.0, glm::vec2()){;}
    FlatDrawable(Shader shader) : FlatDrawable(shader, 1.0, 1.0, glm::vec2()){;}
    FlatDrawable(GLfloat, GLfloat, glm::vec2);
    FlatDrawable(Shader shader, GLfloat, GLfloat, glm::vec2);

    virtual void draw();
    virtual void attachTexture(GLuint);

    virtual void setPosition(glm::vec2 p) {position = p;}
    virtual void setGLCoordinates(glm::vec2, glm::vec2){;}

    void setOpacity(float);
    void setShader(Shader shader);

protected:
    void load(Shader shader, GLfloat, GLfloat, glm::vec2);

    virtual void updateUniformData();

    FlatMesh* mesh;
    GLuint shader_program;

    GLfloat width;
    GLfloat height;
    glm::vec2 position;

    GLuint texture;

    float opacity;

    glm::mat3 transformation;
};

#endif
