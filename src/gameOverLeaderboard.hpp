#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/PALETTE.hpp"
#include <string>
#include "player.hpp"

class GameOverLeaderboard {
    public:
        GameOverLeaderboard();
        void Draw(const Player (&players)[AMOUNT_OF_PLAYERS]) const;
        void DrawWords(const Player (&players)[AMOUNT_OF_PLAYERS]) const;

    private:
        Rectangle outlineRectangle;
        Rectangle hitBox;
};