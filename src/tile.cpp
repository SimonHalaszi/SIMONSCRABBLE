#include "tile.hpp"
#include <cstdio>
#include <iostream>

Tile::Tile()
    : 
    tileIndex({NULL_INDEX, NULL_INDEX, NULL_INDEX}), 
    tilePosition({NULL_POSITION, NULL_POSITION}), 
    inHandPosition(tilePosition), 
    tileColor(DNE_TILE_COLOR), 
    tileOwner(ORPHAN), 
    tileType(DNE), 
    tilePointValue(0), 
    displayPointValue(0),
    tileLetter('\0'), 
    tileMultiplier(0),
    isVowelFlag(false), 
    isTileOccupied(false)
{}

// Initializing the Tile with its position, owner, type, value, letter, and index
Tile::Tile(Vector2 position, Owner tileOwner, Type type, int value, char letterChar, TileIndex tileIndex, int tileMultiplier) {
    this->tilePosition = position;
    this->tileOwner = tileOwner;
    this->tileType = type;
    this->tilePointValue = value;
    this->tileLetter = letterChar;
    this->tileIndex = tileIndex;
    this->tileColor = TILE_COLOR_ARRAY[type];
    this->tileMultiplier = tileMultiplier;

    isVowelFlag = false;
    isTileOccupied = false;

    for(int i = 0; i < VOWELS_COUNT; i++) {
        if(letterChar - 'A' == VOWELS_INDEXES[i]) {
            isVowelFlag = true;
            break;
        }
    }

    displayPointValue = tilePointValue; // Initially, display point value is the same as the actual point value
    inHandPosition = tilePosition; // Initially, in-hand position is the same as the tile position
}

// Tile drawing function
void Tile::Draw() const {
    // Draw board tiles
    if(tileOwner == BOARD) {
        if(isTileOccupied) {
            PlayerTileDraw(); // Occupied board tiles are drawn as player tiles         
            return;
        }
        BoardTileDraw();
        return;
    }
    
    // Draw player tiles
    if(tileOwner == PLAYER) {
        PlayerTileDraw();
    }
}

void Tile::BoardTileDraw() const {
    DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, tileColor);
    char* letterText = nullptr;
    switch(tileType) {
        case DOUBL_LETR:
            letterText = new char[3] {"2L"};
            break;
        case TRIPL_LETR:
            letterText = new char[3] {"3L"};
            break;
        case DOUBL_WORD:
            letterText = new char[3] {"2W"};
            break;
        case TRIPL_WORD:
            letterText = new char[3] {"3W"};
            break;
        case START_TILE:
            letterText = new char[3] {"ST"};
            break;
        default:
            break;
    }
    Vector2 textSize = MeasureTextEx(TILE_FONT, letterText, TILE_FONT_SIZE, TILE_FONT_SPACING);
    DrawTextEx(
        TILE_FONT,
        letterText,
        { 
            tilePosition.x + (TILE_SIZE - textSize.x) / 2, 
            tilePosition.y + (TILE_SIZE - textSize.y) / 2 
        },
        TILE_FONT_SIZE,
        TILE_FONT_SPACING,
        BOARD_TILE_FONT_COLOR
    );
    delete[] letterText;
}

void Tile::PlayerTileDraw() const {
    if(tileType == DNE) {
        DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, tileColor); 
        return; // DNE tiles are not drawn with letters or point values
    }
        
    DrawRectangle(tilePosition.x, tilePosition.y, TILE_SIZE, TILE_SIZE, tileColor);
        
    Vector2 letterTextSize = MeasureTextEx(TILE_FONT, TextFormat("%c", tileLetter), TILE_FONT_SIZE, TILE_FONT_SPACING);
    Vector2 numberTextSize = MeasureTextEx(TILE_FONT, TextFormat("%d", displayPointValue), TILE_FONT_SIZE, TILE_FONT_SPACING);
    DrawTextEx(
        TILE_FONT,
        TextFormat("%c", tileLetter),
        { 
            (tilePosition.x + TILE_FONT_SPACING), 
            tilePosition.y + (TILE_SIZE - letterTextSize.y) 
        },
        TILE_FONT_SIZE,
        TILE_FONT_SPACING,
        PLAYER_TILE_FONT_COLOR
    );
        
    DrawTextEx(
        TILE_FONT,
        TextFormat("%d", displayPointValue),
        { 
            tilePosition.x + (TILE_SIZE - numberTextSize.x) - TILE_FONT_SPACING, 
            tilePosition.y 
        },
        TILE_FONT_SIZE,
        TILE_FONT_SPACING,
        PLAYER_TILE_FONT_COLOR
    );    
}

void Tile::ResetPlayerTileData() {
    tilePosition = inHandPosition;
    tileIndex.boardX = NULL_INDEX;
    tileIndex.boardY = NULL_INDEX;
    displayPointValue = tilePointValue;
    tileMultiplier = 1;
    tileColor = TILE_COLOR;
}

void Tile::ResetTempOccupiedBoardTileData() {    
    tileIndex.handIndex = NULL_INDEX;
    isTileOccupied = false;
}

void Tile::SetPlayerTileDataToTempPlacedState(int boardX, int boardY, Vector2 newPos, int boardTileMultiplier, Color newColor, Type boardTileType) {
    this->ResetPlayerTileData();
    tilePosition = newPos;
    tileIndex.boardX = boardX;
    tileIndex.boardY = boardY;
    if(boardTileType == DOUBL_LETR || boardTileType == TRIPL_LETR) {
        tileMultiplier = boardTileMultiplier;
    } else {
        tileMultiplier = 1;
    }
    if(boardTileType != BOARD_TILE) {
        tileColor = newColor;
    }
}

void Tile::SetBoardTileDataToTempOccupiedState(int handIndex) {
    tileIndex.handIndex = handIndex;
    isTileOccupied = true;
}

void Tile::SetMysteryTile(char letter) {
    // Just in case
    if(tileType == MYSTERY) {
        tileLetter = letter;
    }
}

void Tile::ResetMysteryTile() {
    // Just in case
    if(tileType == MYSTERY) {
        tileLetter = '?';
    }
}

void Tile::DrawTileHighlightAndOutline(Vector2 pos, Color highlightColor, Color outlineColor) const {
    DrawRectangle(
        pos.x, 
        pos.y, 
        TILE_SIZE, 
        TILE_SIZE, 
        highlightColor
    );
    DrawRectangleLinesEx(
        { pos.x - OUTLINE_THICKNESS, 
        pos.y - OUTLINE_THICKNESS, 
        TILE_SIZE + 2 * OUTLINE_THICKNESS, 
        TILE_SIZE + 2 * OUTLINE_THICKNESS }, 
        OUTLINE_THICKNESS,
        outlineColor
    );    
}

void Tile::LockInTempOccupiedBoardTile(const Tile& playerTile) {
    tileIndex.handIndex = NULL_INDEX;
    tileType = BOARD_TILE;
    tilePointValue = playerTile.displayPointValue;
    displayPointValue = playerTile.displayPointValue;
    tileLetter = playerTile.tileLetter;
    tileMultiplier = 1;
    isVowelFlag = playerTile.isVowelFlag;
    isTileOccupied = true;
}
