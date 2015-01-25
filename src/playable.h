// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

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
	
	void select();
	void tempSelect();
	void tempDeSelect();

	float getRadius(){ return radius; }
private:
	void updateUniformData();

	glm::vec2 move_to_position;

	// Not a complete list
	enum class PlayableAttribute { MASSIVE, ARMORED, ARMY, WORKER, FLYING, INVULNERABLE, MECHANICAL };

	// Needs to be static
	Doodad* selection_ring;

	float speed;
	float acceleration;
	float turning_speed;
	float radius;
	float sight_radius;

	int per_level_health_boost;
	int per_level_strength_boost;

	bool selected;
	int temp_selected;

	int level;

	int strength;

	int health;
	int healing_rate;

	std::vector<PlayableAttribute> attributes;

	// Not implemented yet
	// Weapon* weapon
	// Ability* ability

};

#endif