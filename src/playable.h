// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "drawable.h"

class Playable : public Drawable {
public:
	Playable();
	Playable(Mesh*, GLuint, glm::vec3, GLfloat);
private:
};

#endif