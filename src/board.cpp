#include "board.hpp"

Board::Board() {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            Type tileType = BOARD_LAYOUT[i][j];

            // Calculate the position of each tile based on its index
            // and the board's starting position, tile size, and gap
            Vector2 tilePosition = { 
                BOARD_START_X + i * (TILE_SIZE + TILE_GAP), 
                BOARD_START_Y + j * (TILE_SIZE + TILE_GAP)
            };
            
            // Initialize each tile with its position, owner, type, value, letter, and index
            // The value and letter are set to 0 and '\0' respectively for board tiles
            tiles[i][j] = Tile(
                tilePosition, 
                BOARD,  
                tileType, 
                0, 
                '\0', 
                {i, j, NULL_INDEX},
                TILE_TYPE_MULTIPLIERS[tileType]
            );

            if(tileType == START_TILE) {
                startTileIndexing.boardX = i;
                startTileIndexing.boardY = j;
                startTileIndexing.handIndex = NULL_INDEX;
            }
        }
    }

    hitBox = { 
        BOARD_START_X, 
        BOARD_START_Y, 
        BOARD_END_X - BOARD_START_X, 
        BOARD_END_Y - BOARD_START_Y
    };

    outlineRectangle = {
        BOARD_START_X - UI_OUTLINE_THICKNESS,
        BOARD_START_Y - UI_OUTLINE_THICKNESS,
        (BOARD_WIDTH * (TILE_SIZE + TILE_GAP)) + UI_OUTLINE_THICKNESS,
        (BOARD_HEIGHT * (TILE_SIZE + TILE_GAP)) + UI_OUTLINE_THICKNESS
    };
}

// Drawing each tile on the board
void Board::Draw() const {
    DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
    
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            tiles[i][j].Draw();
            if(tiles[i][j].IsCommittedBoardTile()) {
                Vector2 tilePos = tiles[i][j].GetOnScreenPosition();
                // A little thing to make committed tiles pop out
                tiles[i][j].DrawTileHighlightAndOutline(tilePos, COMMITTED_TILE_HIGHLIGHT_COLOR, COMMITTED_TILE_OUTLINE_COLOR);
            }               
        }
    }
}

void Board::ResetAllPlayerOccupiedTiles() {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            if(tiles[i][j].GetHandIndex() != NULL_INDEX) {
                tiles[i][j].ResetTempOccupiedBoardTileData();
            }
        }
    }
}

Tile Board::GetTile(int tileX, int tileY) const { 
    if (tileX < 0 || tileX >= BOARD_WIDTH || tileY < 0 || tileY >= BOARD_HEIGHT) {
        return Tile();
    }
    return tiles[tileX][tileY]; 
}

Tile* Board::GetTilePointer(int tileX, int tileY) {
    if (tileX < 0 || tileX >= BOARD_WIDTH || tileY < 0 || tileY >= BOARD_HEIGHT) {
        return nullptr; // Return null for out-of-bounds access
    }
    return &tiles[tileX][tileY];     
}

const Tile* Board::GetTilePointer(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= BOARD_WIDTH || tileY < 0 || tileY >= BOARD_HEIGHT) {
        return nullptr; // Return null for out-of-bounds access
    }
    return &tiles[tileX][tileY];     
}