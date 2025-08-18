#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/PALETTE.hpp"

class PlayButton {
    public:
        PlayButton();
        void Draw(const bool& isTurnValid) const;
        bool inHitbox(const Vector2& mousePos) const { return CheckCollisionPointRec(mousePos, hitBox); }

    private:
        Rectangle outlineRectangle;
        Rectangle hitBox;
};

