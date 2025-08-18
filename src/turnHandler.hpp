#pragma once
#include <raylib.h>
#include "board.hpp"
#include "player.hpp"
#include "tile.hpp"
#include <iostream>
#include "playButton.hpp"
#include "wordHelpers.hpp"
#include "wordDisplay.hpp"
#include "leaderboard.hpp"
#include "ballot.hpp"
#include "warningBox.hpp"
#include "gameOverLeaderboard.hpp"
#include <unordered_set>
#include <vector>
#include <string>

// TurnHandler class manages all game logic
class TurnHandler {
    public:
    // PUBLIC METHODS
        TurnHandler();
        
        void Draw() const;
        void HandleInputs();

    private:
    // PRIVATE METHODS 
        // Turn management
        void CommitTurn();
        void PlayerSkippedTurn();
        void PlayerRecalledTiles();
        void PlayerVote() { voteInProgress = true; }
        
        // Mouse interaction helpers
        Tile GetBoardTileUnderMouse(Vector2 mousePos) const;
        Tile* GetBoardTileUnderMousePointer(Vector2 mousePos);
        Tile GetPlayerTileUnderMouse(Vector2 mousePos) const;
        Tile* GetPlayerTileUnderMousePointer(Vector2 mousePos);
        Tile* GetPlayerTileUnderMouseOnBoardPointer(Vector2 mousePos);
        Tile* GetPlayerTileUnderMouseInHandPointer(Vector2 mousePos);

        // Drawing helpers
        void DrawTileHighlightAndOutline(const Tile& highlightedTile) const;
        void DrawSelectedTileHighlightAndOutline() const;
        void DrawTilesInTurnHighlightAndOutline() const;

        // Tile selection and placement
        void DeselectSelectedTile();
        void TileSelectingAndPlacing(const Vector2& mousePos);
        void MysteryTileSelection();
        void MysteryTileToHand();
        void HandleSelectingTile(const Vector2& mousePos);
        void SetSelectedTile(Tile& tile) { selectedTile = &tile; }
        bool HandlePlacingTile(const Vector2& mousePos);
        bool CasePlayerTileInHandToBoard(const TileIndex& selectedTileIndexing, const Vector2& mousePos);
        bool CasePlayerTileOnBoardToHand(const TileIndex& selectedTileIndexing, const Vector2& mousePos);
        bool CasePlayerTileOnBoardToElsewhereOnBoard(const TileIndex& selectedTileIndexing, const Vector2& mousePos);

        // Turn update system
        void TurnUpdates();
        void UnapplyMultipliers();
        void ApplyMultipliers();
        
        // Word finding and validation
        void FindWordsInTurn();
        bool AllPlacedTilesInPlayerWord(const TurnWord& playerWord);
        PlayerWordIndexesAndDir FindPlayerWordInfo();
        bool CheckForPlayerWordToRight(int x, int y);
        bool CheckForPlayerWordDown(int x, int y);
        int CountPlayerTilesInDirection(int x, int y, bool isHorizontal);
        TurnWord MakePlayerWord(PlayerWordIndexesAndDir playerWordInfo);
        void MakeBranchWords(TurnWord playerWord, PlayerWordIndexesAndDir playerWordInfo, std::vector<TurnWord>& updateVector);
        TurnWord MakeWordToRight(int x, int y);
        TurnWord MakeWordDown(int x, int y);
        void CalculatePlayerTurnScores();
        bool AreTurnWordsPlacedValid(const std::vector<TurnWord>& updateVector);

    // PRIVATE VARIABLES
        // Shared tile pool for all players
        TilePool tilePool;
        int amountOfTilesLeft;
        // The game board
        Board board;
        // The players in the game
        Player players[AMOUNT_OF_PLAYERS];    
        // Play Button
        PlayButton playButton;
        // Word Display
        WordDisplay wordDisplay;
        // Leaderboard
        Leaderboard leaderBoard;
        // Ballot
        Ballot ballot;
        // Warning Box
        WarningBox warningBox;
        // Game Over Leaderboard
        GameOverLeaderboard gameOverLeaderboard;

        // Flag for a valid turn
        bool isTurnValid;

        // For Player Vote
        bool voteInProgress;
        
        // For Ending Game
        bool lastTurnCycle;
        int playerWithLastTurn;
        bool gameOver;

        // Which player who is currently taking their turn
        int currentPlayerIndex;
        // Current turn number
        int currentTurn;
        // Current player turn score
        int currentPlayerTurnScore;

        // Used for tracking the selected tile
        Tile* selectedTile;

        // Determines whether selecting a mystery tile logic will be in place
        bool isSelectingMysteryTileLetter;

        // Determines whether TurnUpdates() gets ran
        bool turnNeedsUpdated;
        // All the words in this turn
        std::vector<TurnWord> wordsInTurn;
};