#ifndef GameClock_h
#define GameClock_h

#include <SDL2/SDL.h>

#include "debug.h"

class GameClock {
public:
    static GameClock* getInstance();

    void tick();
    void resetAverage();

    float getCurrentTime();
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
