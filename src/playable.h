// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "texture_loader.h"
#include "doodad.h"
#include "mesh.h"
#include "drawable.h"
#include "terrain.h"

class Playable : public Drawable {
public:
	Playable();
	Playable(Mesh*, GLuint, glm::vec3, GLfloat);

	void update(Terrain *);
	void loadFromXML();

	void draw();
private:
	void updateUniformData();

	Doodad* selection_ring;

	float speed;
	float acceleration;
	float turning_speed;
	float radius;
	float sight_radius;

};

#endif