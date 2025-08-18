#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/BOARD_LAYOUT.hpp"
#include "tile.hpp"

// Board class represents the game board
class Board {
    public:
        Board();
        void Draw() const;

        Tile GetTile(int tileX, int tileY) const;
        
        // For getting pointer to a tile
        Tile* GetTilePointer(int tileX, int tileY);
        
        // For getting const pointer to a tile
        const Tile* GetTilePointer(int tileX, int tileY) const;

        void ResetAllPlayerOccupiedTiles();

        TileIndex GetStartTileIndexing() const { return startTileIndexing; }

        bool InHitbox(const Vector2& mousePos) const { return CheckCollisionPointRec(mousePos, hitBox); }
        Rectangle GetHitbox() const { return hitBox; }

    private:
        Rectangle hitBox;
        Rectangle outlineRectangle;
        Vector2 position;
        Tile tiles[BOARD_WIDTH][BOARD_HEIGHT];
        TileIndex startTileIndexing;
};