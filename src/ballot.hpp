#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/PALETTE.hpp"
#include "wordHelpers.hpp"

class Ballot {
    public:
        Ballot();
        void Draw(const std::vector<TurnWord>& wordsInTurn) const;
        bool inYesHitbox(const Vector2& mousePos) { return CheckCollisionPointRec(mousePos, yesButtonHitbox); }
        bool inNoHitbox(const Vector2& mousePos) { return CheckCollisionPointRec(mousePos, noButtonHitbox); }
        void AddYesVote() { yesCount += 1; }
        void AddNoVote() { noCount += 1; }
        int GetYesCount() const { return yesCount; }
        int GetNoCount() const { return noCount; }
        void ResetBallot() { yesCount = 0; noCount = 0; }

    private:
        void DrawWords(const std::vector<TurnWord>& wordsInTurn) const;
        void DrawYesNoButtons() const;

        Rectangle outlineRectangle;
        Rectangle drawBox;
        Rectangle yesButtonHitbox;
        Rectangle noButtonHitbox;
        int yesCount, noCount;
};
