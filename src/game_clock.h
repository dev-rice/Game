#ifndef GameClock_h
#define GameClock_h

#include <GLFW/glfw3.h>

class GameClock {
public:
    static GameClock* getInstance();

    void tick();
private:
    GameClock();

    static GameClock* instance;

    float current_time;
    float last_time;
};

#endif
