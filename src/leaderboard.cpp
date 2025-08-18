#include "leaderboard.hpp"
        
Leaderboard::Leaderboard() {
    hitBox = {
        LEADERBOARD_X, 
        LEADERBOARD_Y, 
        LEADERBOARD_WIDTH, 
        LEADERBOARD_HEIGHT
    };
    outlineRectangle = {
        LEADERBOARD_X - UI_OUTLINE_THICKNESS,
        LEADERBOARD_Y - UI_OUTLINE_THICKNESS,
        LEADERBOARD_WIDTH + (UI_OUTLINE_THICKNESS * 2),
        LEADERBOARD_HEIGHT + (UI_OUTLINE_THICKNESS * 2)
    };
}

void Leaderboard::Draw(const Player (&players)[AMOUNT_OF_PLAYERS]) const {
        DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
        DrawRectangleRec(hitBox, UI_ELEMENT_BASE_COLOR);
        DrawWords(players);
}

void Leaderboard::DrawWords(const Player (&players)[AMOUNT_OF_PLAYERS]) const {
    float drawFromY = hitBox.y + UI_FONT_SPACING;
    float leftSide = hitBox.x + UI_FONT_SPACING;
    float rightSide = hitBox.x + hitBox.width - UI_FONT_SPACING;

    for (int i = 0; i < AMOUNT_OF_PLAYERS; i++) {
        std::string currentPlayerString = "P" + std::to_string(i + 1) + ":";
        int pointValue = players[i].GetScore();

        // Measure word height for vertical spacing
        Vector2 wordTextSize = MeasureTextEx(UI_FONT, currentPlayerString.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);

        // Draw word (left aligned)
        DrawTextEx(
            UI_FONT,
            currentPlayerString.c_str(),
            { leftSide, drawFromY },
            UI_FONT_SIZE,
            UI_FONT_SPACING,
            UI_FONT_COLOR
        );

        // Draw point value (right aligned)
        std::string pointStr = std::to_string(pointValue);
        Vector2 pointTextSize = MeasureTextEx(UI_FONT, pointStr.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);
        DrawTextEx(
            UI_FONT,
            pointStr.c_str(),
            { rightSide - pointTextSize.x, drawFromY },
            UI_FONT_SIZE,
            UI_FONT_SPACING,
            UI_FONT_COLOR
        );

        drawFromY += wordTextSize.y + UI_FONT_SPACING; // Move down for next word
    }
}