// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <stack>

#include "texture_loader.h"
#include "doodad.h"
#include "mesh.h"
#include "drawable.h"
#include "terrain.h"

class Playable : public Drawable {
public:
	

	Playable();
	Playable(Mesh*, GLuint, glm::vec3, GLfloat);

	void update(Terrain *, std::vector<Playable*>);
	void loadFromXML();

	void draw();
	
	void select();
	void deSelect();
	void tempSelect();
	void tempDeSelect();

	void setMovementTarget(glm::vec3);
	void setMovementTargetAndClearStack(glm::vec3);
	void addMovementTarget(glm::vec3);

	bool isMoving();
	bool isSelected(){ return selected; }

	float getRadius(){ return radius; }
private:
	void updateUniformData();

	std::stack<glm::vec3> movement_stack;

	glm::vec3 move_to_position;
	float current_direction;

	// Not a complete list
	enum class PlayableAttribute { MASSIVE, ARMORED, ARMY, WORKER, FLYING, INVULNERABLE, MECHANICAL };

	static Doodad* selection_ring;

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