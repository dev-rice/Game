// playable.h
// Trevor Westphal

#ifndef Playable_h
#define Playable_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <stack>
#include <string>

#include "pugixml.hpp" // PUGI xml library

#include "texture_loader.h"
#include "doodad.h"
#include "mesh.h"
#include "drawable.h"
#include "terrain.h"

class Playable : public Drawable {
public:
	// Not a complete list
	enum class Order{ MOVE, ATTACK, HOLD_POSITION, STOP };
	enum class PlayableAttribute{ MASSIVE, ARMORED, ARMY, WORKER, FLYING, INVULNERABLE, MECHANICAL };

	Playable();
	Playable(Mesh*, GLuint, glm::vec3, GLfloat);

	void update(Terrain *, std::vector<Playable*>);
	void loadFromXML(std::string filepath);

	void draw();

	void select();
	void deSelect();
	void tempSelect();
	void tempDeSelect();

	bool receiveOrder(Playable::Order, glm::vec3, bool);
	void executeOrder(Playable::Order, glm::vec3);

	bool requestPush(glm::vec3);

	void setMovementTarget(glm::vec3);
	void setMovementTargetAndClearStack(glm::vec3);
	void addExternalMovementTarget(glm::vec3);

	void holdPosition();
	void stop();

	bool isMoving();
	bool canBePushed();

	bool isSelected(){ return selected; }
	bool isTempSelected(){ return temp_selected; }

	float getRadius(){ return radius; }

	void setSpeed(float s) {speed = s;}
private:
	void updateUniformData();

	// Movement and interaction variables
	bool has_been_push_requested;
	bool holding_position;

	std::vector<std::tuple<Playable::Order, glm::vec3>> order_queue;

	glm::vec3 move_to_position;
	float movement_target_direction;

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

	int health;
	int healing_rate;

	std::vector<PlayableAttribute> attributes;

	// Not implemented yet
	// Weapon* weapon
	// Ability* ability

};

#endif
