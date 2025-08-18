#pragma once
#include "../config/LETTER_INFO.hpp"
#include "tilePool.hpp"
#include "tile.hpp"

// Player class represents a player in the game
class Player {
    public:
        Player();
        Player(TilePool* tilePool);
        
        void Draw() const;
        void DrawHand() const;
        
        void FillHand();

        Tile GetTile(int tileIndex) const { return playerHand[tileIndex]; }
        
        // Safer alternative that returns nullptr for invalid indices
        Tile* GetTilePointer(int tileIndex);

        void UpdateScore(int newScore) { playerScore += newScore; }
        int GetScore() const { return playerScore; }

        bool InHitbox(const Vector2& mousePos) const { return CheckCollisionPointRec(mousePos, hitBox); }
        Rectangle GetHitbox() const { return hitBox; }

    private:
        Rectangle hitBox;
        Rectangle outlineRectangle;
        TilePool* sharedTilePool;
        Tile playerHand[PLAYER_HAND_SIZE];
        int playerScore;
};
