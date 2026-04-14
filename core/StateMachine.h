#pragma once

enum class GameState {
    MENU = 0,
    RUNNING = 1,
    PAUSED = 2,
    GAME_OVER = 3,
    EXIT = 4
};

class StateMachine {
public:
    StateMachine();

    GameState state() const;
    void requestStart();
    void requestPauseToggle();
    void requestGameOver();
    void requestExit();
    void setState(GameState newState);

private:
    GameState current;
};
