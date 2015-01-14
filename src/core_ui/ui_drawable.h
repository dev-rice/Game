#ifndef UIDrawable_h
#define UIDrawable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>   // std::string
#include <vector>   // std::vector

#include "../pugixml.hpp" // PUGI xml library

#include "../function_helper.h"
#include "../flat_drawable.h"
#include "../window.h"

class UIDrawable : public FlatDrawable {
public:
    UIDrawable(GLuint, GLuint);

    void draw();
    void attachTexture(GLuint);

    glm::vec2 getGLPosition();

    void setPixelCoordinates(int, int, int, int);
    void setGLPosition(glm::vec2);
    void setOutline(bool o) {outline = o;}
    void setGLCoordinates(glm::vec2, glm::vec2);

    virtual void loadFromXML(std::string){;}
protected:
    GLuint shader;

    void parseConstraints(pugi::xml_node);
    virtual bool constraintsAreValid(bool, bool, bool, bool, bool, bool);
    int parseAnchor(const char*, bool);

    bool XOR(bool, bool);

    void updateDimensions();

    int window_width;
    int window_height;

    int width_pixels;
    int height_pixels;

    int x_pixels;
    int y_pixels;

    bool outline;
    glm::mat3 inv_mesh_projection;
};

#endif
