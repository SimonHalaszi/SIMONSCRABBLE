#pragma once
#include "../config/LETTER_INFO.hpp"

// TilePool class manages the pool of tiles available in the game
class TilePool {
    public:
        TilePool() = default;
        void Init() {for(int i = 0; i < TOTAL_LETTERS; i++) bagTileCounts[i] = LETTER_AMOUNTS[i];}
        bool IsLetterAvailable(int letterIndex) const { return bagTileCounts[letterIndex] > 0; }
        bool AreVowelsAvailable() const { for(int i = 0; i < VOWELS_COUNT; i++) { if(bagTileCounts[VOWELS_INDEXES[i]] > 0) return true; } return false;}
        bool LettersAvailable() const { for(int i = 0; i < TOTAL_LETTERS; i++) { if(bagTileCounts[i] > 0) return true; } return false;}
        void TakeLetter(int LetterIndex) { if(bagTileCounts[LetterIndex] > 0) bagTileCounts[LetterIndex]--; }
        int AmountOfLettersLeft() const { int runningTotal = 0; for(int i = 0; i < TOTAL_LETTERS; i++) { runningTotal += bagTileCounts[i]; } return runningTotal; }

    private:
        int bagTileCounts[TOTAL_LETTERS];
};
