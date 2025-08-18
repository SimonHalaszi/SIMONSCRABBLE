#pragma once

enum Owner {
    // Who Owns the Tile
    ORPHAN,
    PLAYER,
    BOARD
};

enum Type {
    // Player Tile Types
    DNE,
    NORMAL,
    MYSTERY,
    
    // Board Tile Types
    BOARD_TILE,
    DOUBL_LETR,
    TRIPL_LETR,
    DOUBL_WORD,
    TRIPL_WORD,
    START_TILE,
};

const int TILE_TYPE_MULTIPLIERS[9] = {
    0, // DNE
    1, // NORMAL
    1, // MYSTERY
    1, // BOARD_TILE
    2, // DOUBL_LETR
    3, // TRIPL_LETR
    2, // DOUBL_WORD
    3, // TRIPL_WORD
    1  // START_TILE
};