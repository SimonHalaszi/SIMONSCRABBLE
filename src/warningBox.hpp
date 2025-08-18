#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/PALETTE.hpp"
#include <string>

class WarningBox {
    public:
        WarningBox();
        void Draw(const WarningType& warningType, const int& amountOfTilesLeft) const;

    private:
        Rectangle outlineRectangle;
        Rectangle hitBox;
};

