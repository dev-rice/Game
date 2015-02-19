// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include "pugixml.hpp" // PUGI xml library

#include "texture_loader.h"
#include "doodad.h"
#include "mesh.h"
#include "drawable.h"
#include "terrain.h"
#include "pathfinder.h"

class Playable : public Drawable {
public:
	// Not a complete list
	enum class Order{ MOVE, ATTACK, HOLD_POSITION, STOP };
	enum class PlayableAttribute{ MASSIVE, ARMORED, ARMY, WORKER, FLYING, INVULNERABLE, MECHANICAL };

	Playable();
	Playable(Mesh*, GLuint, glm::vec3, GLfloat);

	void update(std::vector<Playable*>*);
	void loadFromXML(std::string filepath);

	void draw();

	void select();
	void deSelect();
	void tempSelect();
	void tempDeSelect();

	bool receiveOrder(Playable::Order, glm::vec3, bool, std::vector<glm::vec3>, Playable*);

	void holdPosition();
	void stop();

	bool isSelected(){ return selected; }
	bool isTempSelected(){ return temp_selected; }

	float getRadius(){ return radius; }

private:
	void updateUniformData();

	std::vector<std::tuple<Playable::Order, glm::vec3>> order_queue;

	glm::vec3 target_position;
	float target_direction;

	static Doodad* selection_ring;

	std::string unit_type;

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

	int max_health;
	int health;
	int healing_rate;

	std::vector<PlayableAttribute> attributes;

	// Not implemented yet
	// Weapon* weapon
	// Ability* ability

};

#endif
