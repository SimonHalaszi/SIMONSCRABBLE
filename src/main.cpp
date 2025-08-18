#include "raylib.h"
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/WINDOW_CONSTANTS.hpp"
#include "../config/PALETTE.hpp"
#include "game.hpp"

// Define the extern variables
Font TILE_FONT;
Font UI_FONT;

int main() {

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SimonScrabble");
    SetTargetFPS(30);

    Game game;
    game.Init();

    while (!WindowShouldClose()) {     

        game.HandleInputs();
        
        BeginDrawing();

        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}