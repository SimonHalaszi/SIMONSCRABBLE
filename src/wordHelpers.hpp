#pragma once
#include "../config/GENERAL_CONSTANTS.hpp"
#include "tile.hpp"
#include <vector>
#include <string>

enum WordMultType {
    NORML_WORD_TYPE,
    DOUBL_WORD_TYPE,
    TRIPL_WORD_TYPE
};

struct TurnWord {
    std::vector<Tile*> tilesInWord;
    std::string wordString = "";
    int multiplier = 1;
    int wordValue = 0;
    WordMultType wordMultType = NORML_WORD_TYPE;
};

struct PlayerWordIndexesAndDir {
    int wordRowIndex = NULL_INDEX;
    int wordColIndex = NULL_INDEX;
    bool isHorizontal = false;
};