#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/PALETTE.hpp"
#include "wordHelpers.hpp"

class WordDisplay {
    public:
        WordDisplay();
        void Draw(const bool& isTurnValid, const std::vector<TurnWord>& wordsInTurn) const;
        void DrawWords(const std::vector<TurnWord>& wordsInTurn) const;

    private:
        Rectangle outlineRectangle;
        Rectangle hitBox;
};
