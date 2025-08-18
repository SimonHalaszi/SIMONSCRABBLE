#include "game.hpp"

// Draws the games contents
void Game::Draw() {
    ClearBackground(BACKGROUND_COLOR);
    turnHandler.Draw();
}

// Initialization logic
void Game::Init() {
    // Initializing the TILE_FONT extern variable
    TILE_FONT = GetFontDefault();
    UI_FONT = GetFontDefault();
}

void Game::HandleInputs() {  
    turnHandler.HandleInputs();
}