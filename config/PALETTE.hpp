#pragma once
#include "raylib.h"

inline bool ColorsAreSame(const Color& colorOne, const Color& colorTwo) { return 
    ((colorOne.a == colorTwo.a) &&
    (colorOne.r == colorTwo.r) &&
    (colorOne.g == colorTwo.g) &&
    (colorOne.b == colorTwo.b));
}

const Color NULL_COLOR = {0, 0, 0, 0};

const Color UI_FONT_COLOR = {15, 0, 0, 255};
const Color PLAYER_TILE_FONT_COLOR = {15, 0, 0, 255};
const Color BOARD_TILE_FONT_COLOR = {15, 0, 0, 255};

const Color BACKGROUND_COLOR = {35, 20, 5, 255};
const Color OUTLINE_COLOR = {85, 70, 55, 255};

const Color UI_ELEMENT_BASE_COLOR = {240, 205, 150, 255};

// Play Button Colors
const Color PLAY_BUTTON_CORRECT = {110, 165, 65, 255};
const Color PLAY_BUTTON_WRONG = {235, 55, 70, 255};

// Occupied board tile
const Color COMMITTED_TILE_OUTLINE_COLOR = {235, 55, 70, 255};
const Color COMMITTED_TILE_HIGHLIGHT_COLOR = {255, 255, 255, 40};
// Tile that is selected
const Color SELECTED_TILE_OUTLINE_COLOR = {40, 120, 255, 255};
const Color SELECTED_TILE_HIGHLIGHT_COLOR = {40, 120, 255, 40};
// Turn tiles are the tiles that add to turn
const Color TURN_TILE_OUTLINE_COLOR = {255, 255, 255, 255};
const Color TURN_TILE_HIGHLIGHT_COLOR = {0, 0, 0, 0};
// Tile being hovered over
const Color HIGHLIGHTED_TILE_OUTLINE_COLOR = {255, 210, 40, 255};
const Color HIGHLIGHTED_TILE_COLOR = {255, 210, 40, 40};


const Color DNE_TILE_COLOR = {100, 70, 40, 255};
const Color TILE_COLOR = {255, 240, 215, 255};
const Color DOUBLE_LETTER_TILE_COLOR = {110, 165, 65, 255};
const Color TRIPLE_LETTER_TILE_COLOR = {35, 125, 70, 255};
const Color DOUBLE_WORD_TILE_COLOR = {235, 125, 60, 255};    
const Color TRIPLE_WORD_TILE_COLOR = {215, 80, 40, 255};
const Color DEFAULT_BOARD_TILE_COLOR = {240, 205, 150, 255};
const Color START_TILE_COLOR = {255, 190, 80, 255};

const Color TILE_COLOR_ARRAY[9] {
    DNE_TILE_COLOR,
    TILE_COLOR,
    TILE_COLOR,
    DEFAULT_BOARD_TILE_COLOR,
    DOUBLE_LETTER_TILE_COLOR, 
    TRIPLE_LETTER_TILE_COLOR,
    DOUBLE_WORD_TILE_COLOR,
    TRIPLE_WORD_TILE_COLOR, 
    START_TILE_COLOR
};