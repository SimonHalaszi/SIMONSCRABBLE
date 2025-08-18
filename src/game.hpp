#pragma once
#include "board.hpp"
#include "turnHandler.hpp"
#include "../config/GENERAL_CONSTANTS.hpp"

// Game class manages the game
class Game {
    public:
        Game() : turnHandler() {};
        void Init();
        void Draw();
        void HandleInputs();

    private:
        TurnHandler turnHandler;
};