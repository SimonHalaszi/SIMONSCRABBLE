#include "wordDisplay.hpp"
        
WordDisplay::WordDisplay() {
    hitBox = {
        WORD_DISPLAY_X, 
        WORD_DISPLAY_Y, 
        WORD_DISPLAY_WIDTH, 
        WORD_DISPLAY_HEIGHT
    };
    outlineRectangle = {
        WORD_DISPLAY_X - UI_OUTLINE_THICKNESS,
        WORD_DISPLAY_Y - UI_OUTLINE_THICKNESS,
        WORD_DISPLAY_WIDTH + (UI_OUTLINE_THICKNESS * 2),
        WORD_DISPLAY_HEIGHT + (UI_OUTLINE_THICKNESS * 2)
    };
}

void WordDisplay::Draw(const bool& isTurnValid, const std::vector<TurnWord>& wordsInTurn) const {
        DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
        DrawRectangleRec(hitBox, UI_ELEMENT_BASE_COLOR);
        if(isTurnValid) {
            DrawWords(wordsInTurn);
        }
}

void WordDisplay::DrawWords(const std::vector<TurnWord>& wordsInTurn) const {
    float drawFromY = hitBox.y + UI_FONT_SPACING;
    float leftSide = hitBox.x + UI_FONT_SPACING;
    float rightSide = hitBox.x + hitBox.width - UI_FONT_SPACING;

    for (const TurnWord& wordInTurn : wordsInTurn) {
        std::string currentWordString = wordInTurn.wordString;
        int pointValue = wordInTurn.wordValue;

        // Shorten word if longer than 7 characters
        if (currentWordString.length() > 7) {
            currentWordString = currentWordString.substr(0, 5) + "...";
        }

        // Measure word height for vertical spacing
        Vector2 wordTextSize = MeasureTextEx(UI_FONT, currentWordString.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);

        // Draw word (left aligned)
        DrawTextEx(
            UI_FONT,
            currentWordString.c_str(),
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