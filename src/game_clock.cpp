#include "game_clock.h"

GameClock* GameClock::instance;

GameClock::GameClock(){
    current_time = last_time = glfwGetTime();
}

GameClock* GameClock::getInstance(){
    if(instance){
        return instance;
    } else {
        instance = new GameClock();
        return instance;
    }
}

void GameClock::tick(){
    last_time = current_time;
    current_time = glfwGetTime();
}
