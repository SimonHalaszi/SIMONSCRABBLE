#include "gameOverLeaderboard.hpp"

GameOverLeaderboard::GameOverLeaderboard() {
    hitBox = {
        GAMEOVER_LEADERBOARD_X, 
        GAMEOVER_LEADERBOARD_Y, 
        GAMEOVER_LEADERBOARD_WIDTH, 
        GAMEOVER_LEADERBOARD_HEIGHT
    };
    outlineRectangle = {
        GAMEOVER_LEADERBOARD_X - UI_OUTLINE_THICKNESS,
        GAMEOVER_LEADERBOARD_Y - UI_OUTLINE_THICKNESS,
        GAMEOVER_LEADERBOARD_WIDTH + (UI_OUTLINE_THICKNESS * 2),
        GAMEOVER_LEADERBOARD_HEIGHT + (UI_OUTLINE_THICKNESS * 2)
    };
}

void GameOverLeaderboard::Draw(const Player (&players)[AMOUNT_OF_PLAYERS]) const {
        DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
        DrawRectangleRec(hitBox, UI_ELEMENT_BASE_COLOR);
        DrawWords(players);
}

void GameOverLeaderboard::DrawWords(const Player (&players)[AMOUNT_OF_PLAYERS]) const {
    int playerWhoWon = 0;
    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++) {
        if(players[i].GetScore() > players[playerWhoWon].GetScore()) {
            playerWhoWon = i;
        }
    }

    float drawFromY = hitBox.y + UI_FONT_SPACING;
    float leftSide = hitBox.x + UI_FONT_SPACING;
    float rightSide = hitBox.x + hitBox.width - UI_FONT_SPACING;    

    std::string gameOverStr = "GAME OVER!!!";
    Vector2 gameOverStrSize = MeasureTextEx(UI_FONT, gameOverStr.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);
    DrawTextEx(
        UI_FONT,
        gameOverStr.c_str(),
        { leftSide, drawFromY },
        UI_FONT_SIZE,
        UI_FONT_SPACING,
        UI_FONT_COLOR
    );    
    
    drawFromY += gameOverStrSize.y + UI_FONT_SPACING; // Move down for next word

    std::string playerWhoWonStr = "P" + std::to_string(playerWhoWon + 1) + ": WON!!!";
    Vector2 playerWhoWonStrSize = MeasureTextEx(UI_FONT, playerWhoWonStr.c_str(), UI_FONT_SIZE, UI_FONT_SPACING);
    DrawTextEx(
        UI_FONT,
        playerWhoWonStr.c_str(),
        { leftSide, drawFromY },
        UI_FONT_SIZE,
        UI_FONT_SPACING,
        UI_FONT_COLOR
    );    

    drawFromY += (playerWhoWonStrSize.y * 2) + UI_FONT_SPACING; // Move down for next word

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