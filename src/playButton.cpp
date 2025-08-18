#include "playButton.hpp"

PlayButton::PlayButton() {
    hitBox = {
        PLAY_BUTTON_X, 
        PLAY_BUTTON_Y, 
        PLAY_BUTTON_WIDTH, 
        PLAY_BUTTON_HEIGHT
    };
    outlineRectangle = {
        PLAY_BUTTON_X - UI_OUTLINE_THICKNESS,
        PLAY_BUTTON_Y - UI_OUTLINE_THICKNESS,
        PLAY_BUTTON_WIDTH + (UI_OUTLINE_THICKNESS * 2),
        PLAY_BUTTON_HEIGHT + (UI_OUTLINE_THICKNESS * 2)
    };
}

void PlayButton::Draw(const bool& isTurnValid) const {
        DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
        if(isTurnValid) {
            DrawRectangleRec(hitBox, PLAY_BUTTON_CORRECT);
        } else {
            DrawRectangleRec(hitBox, PLAY_BUTTON_WRONG);
        }
        Vector2 textSize = MeasureTextEx(UI_FONT, "PLAY", UI_FONT_SIZE, UI_FONT_SPACING);
        DrawTextEx(
        UI_FONT,
        "PLAY",
        { 
            hitBox.x + (hitBox.width - textSize.x) / 2, 
            hitBox.y + (hitBox.height - textSize.y) / 2 
        },
        UI_FONT_SIZE,
        UI_FONT_SPACING,
        UI_FONT_COLOR
    );
}