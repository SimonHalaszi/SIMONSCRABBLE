#include "warningBox.hpp"

WarningBox::WarningBox() {
    hitBox = {
        WARNING_BOX_X, 
        WARNING_BOX_Y, 
        WARNING_BOX_WIDTH, 
        WARNING_BOX_HEIGHT
    };
    outlineRectangle = {
        WARNING_BOX_X - UI_OUTLINE_THICKNESS,
        WARNING_BOX_Y - UI_OUTLINE_THICKNESS,
        WARNING_BOX_WIDTH + (UI_OUTLINE_THICKNESS * 2),
        WARNING_BOX_HEIGHT + (UI_OUTLINE_THICKNESS * 2)
    };
}

void WarningBox::Draw(const WarningType& warningType, const int& amountOfTilesLeft) const {
        DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
        if(warningType == SELECT_MYSTERY_TILE) {
            DrawRectangleRec(hitBox, PLAY_BUTTON_CORRECT);

            std::string setMysteryTileStr = std::string("SET\nMYSTERY\nTILE");
            Vector2 setMysteryTileStrSize = MeasureTextEx(UI_FONT, setMysteryTileStr.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);

            DrawTextEx(
                UI_FONT,
                setMysteryTileStr.c_str(),
                { 
                    hitBox.x + (hitBox.width - setMysteryTileStrSize.x) / 2, 
                    hitBox.y - (hitBox.height - setMysteryTileStrSize.y) / 2 
                },
                UI_FONT_SIZE,
                UI_FONT_SPACING,
                UI_FONT_COLOR
            );
        } else if(warningType == LAST_TURN) {
            DrawRectangleRec(hitBox, PLAY_BUTTON_WRONG);
            Vector2 textSize = MeasureTextEx(UI_FONT, "LAST TURN", UI_FONT_SIZE, UI_FONT_SPACING);
            DrawTextEx(
                UI_FONT,
                "LAST TURN",
                { 
                    hitBox.x + (hitBox.width - textSize.x) / 2, 
                    hitBox.y + (hitBox.height - textSize.y) / 2 
                },
                UI_FONT_SIZE,
                UI_FONT_SPACING,
                UI_FONT_COLOR
            );
        } else if(warningType == TILES_LEFT_REMINDER){
            DrawRectangleRec(hitBox, UI_ELEMENT_BASE_COLOR);
            std::string tilesLeftStr = std::to_string(amountOfTilesLeft);
            Vector2 textSize = MeasureTextEx(UI_FONT, tilesLeftStr.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);
            DrawTextEx(
                UI_FONT,
                tilesLeftStr.c_str(),
                { 
                    hitBox.x + (hitBox.width - textSize.x) / 2, 
                    hitBox.y + (hitBox.height - textSize.y) / 2 
                },
                UI_FONT_SIZE,
                UI_FONT_SPACING,
                UI_FONT_COLOR
            );
        }
}