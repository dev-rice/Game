// mover.h
// Trevor Westphal

class Mover {
public:
	Mover();

	enum MovementType{ LAND, AIR, WATER };
	enum ReturnedOrder{ TURN_CCW, TURN_CW, NO_TURN, WAIT};

private:
	// land units avoid other land units and cliffs, blocked terrain
	// air units avoid other air units
	// water units avoid other water units and land
};