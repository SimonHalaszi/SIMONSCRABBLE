#pragma once
#include <raylib.h>
#include "../config/GENERAL_CONSTANTS.hpp"
#include "../config/TILE_INFO.hpp"
#include "../config/PALETTE.hpp"
#include "../config/LETTER_INFO.hpp"

// This struct represents the index of a tile in the game
// It contains the board coordinates and/or the index in the player's hand
struct TileIndex {
    int boardX = NULL_INDEX;
    int boardY = NULL_INDEX;
    int handIndex = NULL_INDEX;
};

// This class represents a tile in the game
class Tile {
    public:
        // CONSTRUCTORS
        Tile();
        Tile(Vector2 position, Owner tileOwner, Type type, int value, char letterChar, TileIndex tileIndex, int tileMultiplier);
        
        // CORE RENDERING
        void Draw() const;
        void DrawTileHighlightAndOutline(Vector2 pos, Color highlightColor, Color outlineColor) const;
        
        // POSITION ACCESSORS
        Vector2 GetOnScreenPosition() const { return tilePosition; }
        Vector2 GetInHandPosition() const { return inHandPosition; }
        void PositionBackInHand() { tilePosition = inHandPosition; }
        
        // TILE PROPERTIES ACCESSORS
        char GetLetter() const { return tileLetter; }
        bool IsVowel() const { return isVowelFlag; }
        Owner GetOwner() const { return tileOwner; }
        Type GetType() const { return tileType; }
        int GetMultiplier() const { return tileMultiplier; }
        Color GetColor() const { return tileColor; }
        void SetColor(const Color& newColor) { tileColor = newColor; }
        TileIndex GetTileIndexing() const { return tileIndex; }
        int GetHandIndex() const { return tileIndex.handIndex; }
        
        // SCORING PROPERTIES
        int GetTilePointValue() const { return tilePointValue; }
        int GetDisplayPointValue() const { return displayPointValue; }
        void DoMultiplier(int multiplier) { displayPointValue *= multiplier; }
        void SetDisplayValue(int givenValue) { displayPointValue = givenValue; }
        
        // TILE STATE QUERIES
        bool IsDNE() const { return tileType == DNE; }
        bool IsOccupied() const { return isTileOccupied; }
        bool IsPlayerTilePlaced() const { return tileOwner == PLAYER && (tileIndex.boardX != NULL_INDEX || tileIndex.boardY != NULL_INDEX); }
        bool IsOccupiedBoardTileBecausePlayer() const { return isTileOccupied && tileIndex.handIndex != NULL_INDEX; }
        bool IsCommittedBoardTile() const { return tileOwner == BOARD && isTileOccupied && tileIndex.handIndex == NULL_INDEX; }
        bool NeedsReturnedToHand() const { return tileOwner == PLAYER && (tileIndex.boardX != NULL_INDEX || tileIndex.boardY != NULL_INDEX); }
        
        // TILE STATE MANAGEMENT
        void SetOccupied(bool occupied) { isTileOccupied = occupied; }
        void ResetPlayerTileData();
        void ResetTempOccupiedBoardTileData();
        void SetPlayerTileDataToTempPlacedState(int boardX, int boardY, Vector2 newPos, int boardTileMultiplier, Color newColor, Type boardTileType);
        void SetBoardTileDataToTempOccupiedState(int handIndex);
        void LockInTempOccupiedBoardTile(const Tile& playerTile);
        
        // MYSTERY TILE MANAGEMENT
        void ResetMysteryTile();
        void SetMysteryTile(char letter);

    private:   
        // POSITION DATA
        TileIndex tileIndex; // Index information for the tile
        Vector2 tilePosition;  // Position of the tile on the screen
        Vector2 inHandPosition; // Only used for player tiles

        // TILE PROPERTIES
        Color tileColor;
        Owner tileOwner;
        Type tileType;
        
        // SCORING DATA (ordered to match constructor initialization)
        int tilePointValue;    // Actual point value of the tile 
        int displayPointValue; // When multiplier tiles are used, this is the value displayed on the tile
        
        // REMAINING PROPERTIES
        char tileLetter;
        int tileMultiplier; // Only used for board tiles
        
        // FLAGS
        bool isVowelFlag;
        bool isTileOccupied; // Used for board tiles only

        // PRIVATE RENDERING METHODS
        void BoardTileDraw() const;
        void PlayerTileDraw() const;
};      