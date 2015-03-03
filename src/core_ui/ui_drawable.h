#ifndef UIDrawable_h
#define UIDrawable_h

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>   // std::string
#include <vector>   // std::vector

#include "pugixml.hpp" // PUGI xml library

#include "function_helper.h"
#include "flat_drawable.h"
#include "window.h"

class UIDrawable : public FlatDrawable {
public:
    UIDrawable(GLuint);
    UIDrawable(GLuint, GLuint);

    void draw();
    void attachTexture(GLuint);

    glm::vec2 getGLPosition();

    void setPixelCoordinates(int, int, int, int);
    void setGLPosition(glm::vec2);
    void setOutline(bool o) {outline = o;}
    void setGLCoordinates(glm::vec2, glm::vec2);

    void setParent(UIDrawable*);

    int getXPixels(){ return x_pixels;}
    int getYPixels(){ return y_pixels;}
    int getWidthPixels(){ return width_pixels;}
    int getHeightPixels(){ return height_pixels;}

    virtual void loadFromXML(std::string){;}
    virtual void receiveNotification(UIDrawable*);

protected:
    void load(GLuint);

    GLuint shader;

    void parseConstraints(pugi::xml_node);
    virtual bool constraintsAreValid(bool, bool, bool, bool, bool, bool);
    virtual void didLoadXML(){;}

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

    UIDrawable* parent;
};

#endif
