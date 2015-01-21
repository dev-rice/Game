// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "drawable.h"
#include "terrain.h"

class Playable : public Drawable {
public:
	Playable();
	Playable(Mesh*, GLuint, glm::vec3, GLfloat);

	void update(Terrain *);
private:
	void updateUniformData();
};

#endif