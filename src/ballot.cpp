#include "ballot.hpp"
        
Ballot::Ballot() {
    drawBox = {
        BALLOT_START_X, 
        BALLOT_START_Y, 
        BALLOT_WIDTH, 
        BALLOT_HEIGHT
    };
    outlineRectangle = {
        BALLOT_START_X - UI_OUTLINE_THICKNESS,
        BALLOT_START_Y - UI_OUTLINE_THICKNESS,
        BALLOT_WIDTH + (UI_OUTLINE_THICKNESS * 2),
        BALLOT_HEIGHT + (UI_OUTLINE_THICKNESS * 2)
    };
    yesButtonHitbox = {
        BALLOT_YES_X,
        BALLOT_YES_Y,
        BALLOT_YESNO_WIDTH,
        BALLOT_YESNO_HEIGHT
    };
    noButtonHitbox = {
        BALLOT_NO_X,
        BALLOT_NO_Y,
        BALLOT_YESNO_WIDTH,
        BALLOT_YESNO_HEIGHT
    };
    noCount = 0;
    yesCount = 0;
}

void Ballot::Draw(const std::vector<TurnWord>& wordsInTurn) const {
        DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
        DrawRectangleRec(drawBox, UI_ELEMENT_BASE_COLOR);
        DrawWords(wordsInTurn);
        DrawYesNoButtons();
}

void Ballot::DrawWords(const std::vector<TurnWord>& wordsInTurn) const {
    float drawFromY = drawBox.y + UI_FONT_SPACING;
    float leftSide = drawBox.x + UI_FONT_SPACING;
    float rightSide = drawBox.x + drawBox.width - UI_FONT_SPACING;

    for (const TurnWord& wordInTurn : wordsInTurn) {
        std::string currentWordString = wordInTurn.wordString;
        int pointValue = wordInTurn.wordValue;

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

void Ballot::DrawYesNoButtons() const {
    DrawRectangleRec(noButtonHitbox, PLAY_BUTTON_WRONG);
    Vector2 textSize = MeasureTextEx(UI_FONT, "NO", UI_FONT_SIZE, UI_FONT_SPACING);
    DrawTextEx(
        UI_FONT,
        "NO",
        { 
            noButtonHitbox.x + (noButtonHitbox.width - textSize.x) / 2, 
            noButtonHitbox.y + (noButtonHitbox.height - textSize.y) / 2 
        },
        UI_FONT_SIZE,
        UI_FONT_SPACING,
        UI_FONT_COLOR
    );
    
    DrawRectangleRec(yesButtonHitbox, PLAY_BUTTON_CORRECT);
    textSize = MeasureTextEx(UI_FONT, "YES", UI_FONT_SIZE, UI_FONT_SPACING);
    DrawTextEx(
        UI_FONT,
        "YES",
        { 
            yesButtonHitbox.x + (yesButtonHitbox.width - textSize.x) / 2, 
            yesButtonHitbox.y + (yesButtonHitbox.height - textSize.y) / 2 
        },
        UI_FONT_SIZE,
        UI_FONT_SPACING,
        UI_FONT_COLOR
    );
}