#include "core/StateMachine.h"

StateMachine::StateMachine() : current(GameState::MENU) {
}

GameState StateMachine::state() const {
    return current;
}

void StateMachine::requestStart() {
    if (current == GameState::MENU || current == GameState::GAME_OVER) {
        current = GameState::RUNNING;
    }
}

void StateMachine::requestPauseToggle() {
    if (current == GameState::RUNNING) {
        current = GameState::PAUSED;
    } else if (current == GameState::PAUSED) {
        current = GameState::RUNNING;
    }
}

void StateMachine::requestGameOver() {
    if (current == GameState::RUNNING) {
        current = GameState::GAME_OVER;
    }
}

void StateMachine::requestExit() {
    current = GameState::EXIT;
}

void StateMachine::setState(GameState newState) {
    current = newState;
}
