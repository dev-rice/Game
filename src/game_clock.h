#ifndef GameClock_h
#define GameClock_h

#include <GLFW/glfw3.h>

class GameClock {
public:
    static GameClock* getInstance();

    void tick();
    void resetAverage();

    float getDeltaTime();
    float getAverageDeltaTime();


private:
    GameClock();

    static GameClock* instance;

    float current_time;
    float last_time;
    float delta_time;

    int tick_count;
    float average_delta_time;

};

#endif
