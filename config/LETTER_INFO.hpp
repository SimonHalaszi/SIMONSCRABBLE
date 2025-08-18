#pragma once
#include "GENERAL_CONSTANTS.hpp"

const int TOTAL_LETTERS = 27; // 26 letters + 1 for blank tiles

const int VOWELS_COUNT = 6; // A, E, I, O, U, Y
const int VOWELS_INDEXES[VOWELS_COUNT] = {0, 4, 8, 14, 20, 24};

// If you want to include the mystery tile as a vowel, uncomment the line below

// const int VOWELS_COUNT = 7; // A, E, I, O, U, Y, ? (mystery tile)
// const int VOWELS_INDEXES[VOWELS_COUNT] = {0, 4, 8, 14, 20, 24, 26};

const int MYSTERY_TILE_INDEX = 26; // Index for the mystery tile

const int VOWEL_TRY_COUNT = static_cast<int>(PLAYER_HAND_SIZE / 3); // Number of vowels to try to get in the player's hand

const int LETTER_AMOUNTS[27] = {
    9, // A
    2, // B
    2, // C
    4, // D
    12, // E
    2, // F
    3, // G
    2, // H
    9, // I
    1, // J
    1, // K
    4, // L
    2, // M
    6, // N
    8, // O
    2, // P
    1, // Q
    6, // R
    4, // S
    6, // T
    4, // U
    2, // V
    2, // W
    1, // X
    2, // Y
    1, // Z
    2 // Blank tiles
};

const int LETTER_SCORE_VALUES[27] = {
    1, // A
    3, // B
    3, // C
    2, // D
    1, // E
    4, // F
    2, // G
    4, // H
    1, // I
    8, // J
    5, // K
    2, // L
    3, // M
    1, // N
    1, // O
    3, // P
    10, // Q
    1, // R
    1, // S
    1, // T
    1, // U
    4, // V
    4, // W
    8, // X
    4, // Y
    10, // Z
    0 // Blank tiles have no value
};

const char LETTER_INDEXES[27] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
    'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '?'
};