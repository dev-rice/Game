#ifndef UIDrawable_h
#define UIDrawable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "flat_drawable.h"

class UIDrawable : public FlatDrawable {
public:
    UIDrawable(FlatMesh*, GLFWwindow*, GLuint, GLuint);

    void setPosition(glm::vec2);
    void attachTexture(GLuint);
private:

    glm::mat3 mesh_projection;
};

#endif