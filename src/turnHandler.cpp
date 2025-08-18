#include "turnHandler.hpp"

// PUBLIC METHODS

TurnHandler::TurnHandler() {
    currentPlayerIndex = 0;
    currentTurn = 1;
    selectedTile = nullptr;
    isSelectingMysteryTileLetter = false;
    isTurnValid = false;
    turnNeedsUpdated = false;
    wordsInTurn = std::vector<TurnWord>{};
    voteInProgress = false;
    lastTurnCycle = false;
    playerWithLastTurn = NULL_INDEX;
    gameOver = false;

    // Initialize the tile pool
    tilePool.Init();

    // Initialize players with the shared tile pool
    for (int i = 0; i < AMOUNT_OF_PLAYERS; i++) {
        players[i] = Player(&tilePool);
        players[i].FillHand();
    }

    amountOfTilesLeft = tilePool.AmountOfLettersLeft();
}

void TurnHandler::Draw() const {
    Vector2 mousePos = GetMousePosition();
    if(!gameOver) {
        if(!voteInProgress) {
            board.Draw(); // Draw the board
            players[currentPlayerIndex].Draw(); // Draw the current player's hand

            // Drawing a highlight over the tile the mouse is hovering over
            
            DrawTilesInTurnHighlightAndOutline();
            
            if(board.InHitbox(mousePos)) {
                DrawTileHighlightAndOutline(GetBoardTileUnderMouse(mousePos));
            } 
            else if(players[currentPlayerIndex].InHitbox(mousePos)) {
                DrawTileHighlightAndOutline(GetPlayerTileUnderMouse(mousePos));
            }

            if(selectedTile) {
                DrawSelectedTileHighlightAndOutline();
            }        
        }

        playButton.Draw(isTurnValid);
        wordDisplay.Draw(isTurnValid, wordsInTurn);
        leaderBoard.Draw(players);
        
        if(voteInProgress) {
            ballot.Draw(wordsInTurn);
        }

        if(amountOfTilesLeft != 0) {
            warningBox.Draw(TILES_LEFT_REMINDER, amountOfTilesLeft);
        }
        // Selecting tile warning overrides tiles left reminder
        if(isSelectingMysteryTileLetter) {
            warningBox.Draw(SELECT_MYSTERY_TILE, amountOfTilesLeft);
        }
        // Last Turn warning overwrites the select mystery tile warning
        if(lastTurnCycle) {
            warningBox.Draw(LAST_TURN, amountOfTilesLeft);
        }
    }

    if(gameOver) {
        board.Draw();
        gameOverLeaderboard.Draw(players);
    }
}

void TurnHandler::HandleInputs() {
    Vector2 mousePos = GetMousePosition();

    if(!gameOver) {
        if(!voteInProgress) {
            // Deselect tile with right click
            if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                DeselectSelectedTile();
            }
            
            // Selecting and placing tiles
            if(!isSelectingMysteryTileLetter) {
                TileSelectingAndPlacing(mousePos);
            }

            // Selecting letter for mystery tile
            if(isSelectingMysteryTileLetter && selectedTile) {
                MysteryTileSelection();
            }

            // Skipping turn with space key
            if(IsKeyPressed(KEY_SPACE)) {
                PlayerSkippedTurn();
            }

            // Recall turn tiles
            if(IsKeyPressed(KEY_BACKSPACE)) {
                PlayerRecalledTiles();
            }

            // Turn updates
            if(turnNeedsUpdated) {
                TurnUpdates();
                turnNeedsUpdated = false;
            }

            
            // CommitTurn Logic
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) 
                && isTurnValid 
                && playButton.inHitbox(mousePos) 
            ) {
                PlayerVote();
            }
        }
        
        if(voteInProgress) {
            if(ballot.inYesHitbox(mousePos) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                ballot.AddYesVote();
            }
            if(ballot.inNoHitbox(mousePos) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                ballot.AddNoVote();
            }
            if(ballot.GetYesCount() == AMOUNT_OF_PLAYERS - 1) {
                CommitTurn();
                voteInProgress = false;
                ballot.ResetBallot();
            }
            if(ballot.GetNoCount() > 0) {
                voteInProgress = false;
                ballot.ResetBallot();
            }
        }
    }
}

// PRIVATE METHODS

// Turn management
void TurnHandler::CommitTurn() { 
    for(int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        
        if(currentTile.IsPlayerTilePlaced()) {
            TileIndex currentTileIndexing = currentTile.GetTileIndexing();
            Tile* boardTilePtr = board.GetTilePointer(currentTileIndexing.boardX, currentTileIndexing.boardY);
            if (!boardTilePtr) {
                continue; // Skip invalid tiles
            }
            Tile& boardTile = *boardTilePtr;
            boardTile.LockInTempOccupiedBoardTile(currentTile);

            currentTile = Tile(
                {NULL_POSITION, NULL_POSITION},
                PLAYER,
                DNE,
                0,
                '\0',
                {NULL_INDEX, NULL_INDEX, i},
                TILE_TYPE_MULTIPLIERS[DNE]
            );
         }
    }
    players[currentPlayerIndex].UpdateScore(currentPlayerTurnScore);

    selectedTile = nullptr;

    // Refill the player's hand with new tiles
    players[currentPlayerIndex].FillHand();
    amountOfTilesLeft = tilePool.AmountOfLettersLeft();

    wordsInTurn.clear();
    isTurnValid = !wordsInTurn.empty();

    if(lastTurnCycle && currentPlayerIndex == playerWithLastTurn) {
        gameOver = true;
        return;
    }

    if(!tilePool.LettersAvailable() && lastTurnCycle == false) {
        lastTurnCycle = true;
        playerWithLastTurn = currentPlayerIndex;
    }

    currentPlayerIndex = (currentPlayerIndex + 1) % AMOUNT_OF_PLAYERS;
    currentTurn++;
}

void TurnHandler::PlayerSkippedTurn() {
    for(int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        
        if(currentTile.NeedsReturnedToHand()) {
            currentTile.ResetPlayerTileData();
        }
        if(currentTile.GetType() == MYSTERY) {
            currentTile.ResetMysteryTile();
        }
    }
    
    board.ResetAllPlayerOccupiedTiles();

    selectedTile = nullptr;

    UnapplyMultipliers();
    wordsInTurn.clear();
    CalculatePlayerTurnScores();
    isTurnValid = !wordsInTurn.empty();

    if(lastTurnCycle && currentPlayerIndex == playerWithLastTurn) {
        gameOver = true;
        return;
    }

    if(currentTurn == 1) {
        currentPlayerIndex = (currentPlayerIndex + 1) % AMOUNT_OF_PLAYERS;
        return;
    } else {
        currentPlayerIndex = (currentPlayerIndex + 1) % AMOUNT_OF_PLAYERS;
        currentTurn++;
        return;
    }
}

void TurnHandler::PlayerRecalledTiles() {
    for(int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        
        if(currentTile.NeedsReturnedToHand()) {
            currentTile.ResetPlayerTileData();
        }
        if(currentTile.GetType() == MYSTERY) {
            currentTile.ResetMysteryTile();
        }
    }

    selectedTile = nullptr;

    UnapplyMultipliers();
    board.ResetAllPlayerOccupiedTiles();
    wordsInTurn.clear();
    CalculatePlayerTurnScores();
    isTurnValid = !wordsInTurn.empty();
}

// Mouse interaction helpers
Tile TurnHandler::GetBoardTileUnderMouse(Vector2 mousePos) const {
    // In case this is called incorrectly
    if(!board.InHitbox(mousePos)) {
        return Tile();
    }    
    
    Rectangle boardHitBox = board.GetHitbox();

    mousePos.x -= boardHitBox.x;
    mousePos.y -= boardHitBox.y;

    int tileX = static_cast<int>(mousePos.x) / (TILE_SIZE + TILE_GAP);
    int tileY = static_cast<int>(mousePos.y) / (TILE_SIZE + TILE_GAP);

    return board.GetTile(tileX, tileY);
}

Tile* TurnHandler::GetBoardTileUnderMousePointer(Vector2 mousePos) {
    // In case this is called incorrectly
    if(!board.InHitbox(mousePos)) {
        return nullptr;
    }
    
    Rectangle boardHitBox = board.GetHitbox();

    mousePos.x -= boardHitBox.x;
    mousePos.y -= boardHitBox.y;

    int tileX = static_cast<int>(mousePos.x) / (TILE_SIZE + TILE_GAP);
    int tileY = static_cast<int>(mousePos.y) / (TILE_SIZE + TILE_GAP);

    return board.GetTilePointer(tileX, tileY);
}

Tile TurnHandler::GetPlayerTileUnderMouse(Vector2 mousePos) const {
    // In case this is called incorrectly
    if(!players[currentPlayerIndex].InHitbox(mousePos)) {
        return Tile();
    }
    
    Rectangle playerHandHitBox = players[currentPlayerIndex].GetHitbox();

    mousePos.x -= playerHandHitBox.x;

    int tileIndex = static_cast<int>(mousePos.x) / (TILE_SIZE + TILE_GAP);

    return players[currentPlayerIndex].GetTile(tileIndex);
}

Tile* TurnHandler::GetPlayerTileUnderMousePointer(Vector2 mousePos) {
    // In case this is called incorrectly
    if(!players[currentPlayerIndex].InHitbox(mousePos)) {
        return nullptr;
    }
    
    Rectangle playerHandHitBox = players[currentPlayerIndex].GetHitbox();

    mousePos.x -= playerHandHitBox.x;

    int tileIndex = static_cast<int>(mousePos.x) / (TILE_SIZE + TILE_GAP);

    return players[currentPlayerIndex].GetTilePointer(tileIndex);
}

Tile* TurnHandler::GetPlayerTileUnderMouseOnBoardPointer(Vector2 mousePos) {
    // In case this is called incorrectly
    if(!board.InHitbox(mousePos)) {
        return nullptr;
    }    
    
    Rectangle boardHitBox = board.GetHitbox();

    mousePos.x -= boardHitBox.x;
    mousePos.y -= boardHitBox.y;

    int tileX = static_cast<int>(mousePos.x) / (TILE_SIZE + TILE_GAP);
    int tileY = static_cast<int>(mousePos.y) / (TILE_SIZE + TILE_GAP);

    Tile tempTile = board.GetTile(tileX, tileY);
    // Board tile isnt occupied so no player tile to select
    if(!tempTile.IsOccupied()) {
        return nullptr;
    }

    int tileIndex = tempTile.GetHandIndex();

    return players[currentPlayerIndex].GetTilePointer(tileIndex);
}

Tile* TurnHandler::GetPlayerTileUnderMouseInHandPointer(Vector2 mousePos) {
    // In case this is called incorrectly
    if(!players[currentPlayerIndex].InHitbox(mousePos)) {
        return nullptr;
    }
    
    Rectangle playerHandHitBox = players[currentPlayerIndex].GetHitbox();
    
    mousePos.x -= playerHandHitBox.x;

    int tileIndex = static_cast<int>(mousePos.x) / (TILE_SIZE + TILE_GAP);

    Tile* tempTilePtr = players[currentPlayerIndex].GetTilePointer(tileIndex);
    if (!tempTilePtr) {
        return nullptr;
    }
    Tile& tempTile = *tempTilePtr;
    TileIndex tempIndexes = tempTile.GetTileIndexing();
    // Tile is on the board currently and shouldnt be selectable by pressing the empty space on the hand
    if(tempIndexes.boardX != NULL_INDEX || tempIndexes.boardY != NULL_INDEX) {
        return nullptr;
    }

    return tempTilePtr;
}

// Drawing helpers
void TurnHandler::DrawTileHighlightAndOutline(const Tile& highlightedTile) const {
    // Drawing highlight of emptied player tile
    if(highlightedTile.IsPlayerTilePlaced()) {
            highlightedTile.DrawTileHighlightAndOutline(
                highlightedTile.GetInHandPosition(), 
                HIGHLIGHTED_TILE_COLOR, 
                HIGHLIGHTED_TILE_OUTLINE_COLOR
            );
        return;
    }
    
    highlightedTile.DrawTileHighlightAndOutline(
        highlightedTile.GetOnScreenPosition(), 
        HIGHLIGHTED_TILE_COLOR, 
        HIGHLIGHTED_TILE_OUTLINE_COLOR
    );
}

void TurnHandler::DrawSelectedTileHighlightAndOutline() const {
    if(selectedTile != nullptr) {
        selectedTile->DrawTileHighlightAndOutline(
            selectedTile->GetOnScreenPosition(), 
            SELECTED_TILE_HIGHLIGHT_COLOR, 
            SELECTED_TILE_OUTLINE_COLOR
        );
    }
}

void TurnHandler::DrawTilesInTurnHighlightAndOutline() const {
    for(const TurnWord& wordInTurn : wordsInTurn) {
        for(const Tile* tile : wordInTurn.tilesInWord) {
            tile->DrawTileHighlightAndOutline(
                tile->GetOnScreenPosition(),
                TURN_TILE_HIGHLIGHT_COLOR,
                TURN_TILE_OUTLINE_COLOR
            );
        }
    }
}

// Tile selection and placement
void TurnHandler::DeselectSelectedTile() {
    if(isSelectingMysteryTileLetter && selectedTile) {
        selectedTile->ResetMysteryTile();
        // If a mystery tile that is placed and hasnt had its letter selected yet is deselected its returned to hand
        if(selectedTile->IsPlayerTilePlaced()) {
            MysteryTileToHand();
            turnNeedsUpdated = true;
        }
        isSelectingMysteryTileLetter = false;
    }
    selectedTile = nullptr;
}

void TurnHandler::TileSelectingAndPlacing(const Vector2& mousePos) {
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if(!selectedTile) {
            HandleSelectingTile(mousePos);
        } else {
            // Only update turn if tile was actually moved
            if(HandlePlacingTile(mousePos)) {
                turnNeedsUpdated = true;
            }
        }
    }
}

void TurnHandler::MysteryTileSelection() {
    if(!selectedTile) return; // Safety check
    
    if(selectedTile->IsPlayerTilePlaced()) {
        selectedTile->ResetMysteryTile();
        MysteryTileToHand();
        turnNeedsUpdated = true;
    }      
    int key = GetKeyPressed();
    if((key >= KEY_A && key <= KEY_Z) || key == KEY_SPACE) {
        char selectedChar = static_cast<char>(key);
        selectedTile->SetMysteryTile(selectedChar);
        isSelectingMysteryTileLetter = false;
    }    
}

void TurnHandler::MysteryTileToHand() {
    if(selectedTile) {
        int boardTileX = selectedTile->GetTileIndexing().boardX;
        int boardTileY = selectedTile->GetTileIndexing().boardY;
        Tile* boardTilePtr = board.GetTilePointer(boardTileX, boardTileY);
        if(boardTilePtr) {
            boardTilePtr->ResetTempOccupiedBoardTileData();
            selectedTile->ResetPlayerTileData();
        }
    }
}

void TurnHandler::HandleSelectingTile(const Vector2& mousePos) {
    if(board.InHitbox(mousePos)) {
        Tile* boardTilePtr = GetBoardTileUnderMousePointer(mousePos);
        if (!boardTilePtr) return;
        Tile& boardTile = *boardTilePtr;
        // Only allow selection if this is a temporary player tile, not a committed one
        if(boardTile.IsOccupied() && boardTile.IsOccupiedBoardTileBecausePlayer()) {
            Tile* playerTilePtr = GetPlayerTileUnderMouseOnBoardPointer(mousePos);
            if (playerTilePtr) {
                SetSelectedTile(*playerTilePtr);
                if(selectedTile->GetType() == MYSTERY) {
                    isSelectingMysteryTileLetter = true;
                    selectedTile->ResetMysteryTile();
                }        
                if(selectedTile->IsDNE()) {
                    selectedTile = nullptr;
                }
            }
        }
    // If clicking on empty board tile or committed tile, don't select anything
    } else if(players[currentPlayerIndex].InHitbox(mousePos)) {
        Tile* handTilePtr = GetPlayerTileUnderMouseInHandPointer(mousePos);
        if (handTilePtr) {
            SetSelectedTile(*handTilePtr);
            // If the attempted selection is invalid
            if(selectedTile->GetType() == MYSTERY) {
                isSelectingMysteryTileLetter = true;
                selectedTile->ResetMysteryTile();
            }        
            if(selectedTile->IsDNE()) {
                selectedTile = nullptr;
            }
        }
    }
}

bool TurnHandler::HandlePlacingTile(const Vector2& mousePos) {
    // Case 1: Player Selects Another Placed Player Tile or A Committed Board Tile
    Tile* boardTilePtr = GetBoardTileUnderMousePointer(mousePos);
    if(selectedTile && boardTilePtr && boardTilePtr->IsOccupied()) {
        Tile& boardTile = *boardTilePtr;
        // Only allow selection if this is a temporary player tile, not a committed one
        if(boardTile.IsOccupiedBoardTileBecausePlayer()) {
            Tile* playerTilePtr = GetPlayerTileUnderMouseOnBoardPointer(mousePos);
            if (playerTilePtr) {
                SetSelectedTile(*playerTilePtr);
            }
            return false; // No tile moved, just selection changed
        }
        // If it's a committed tile, do nothing
        return false;
    }
    
    // Case 2: Player Selects A Different Tile in Hand
    if(selectedTile && players[currentPlayerIndex].InHitbox(mousePos)) {
        Tile* handTilePtr = GetPlayerTileUnderMouseInHandPointer(mousePos);
        if(handTilePtr && handTilePtr->GetType() != DNE) {
            SetSelectedTile(*handTilePtr);
            return false; // No tile moved, just selection changed
        }
    }
    
    TileIndex selectedTileIndexing;
    if(selectedTile) {
        selectedTileIndexing = selectedTile->GetTileIndexing();
    }
    
    // Case 3: Player Tile In Hand to Board
    if(
        selectedTile
        && selectedTileIndexing.boardX == NULL_INDEX 
        && selectedTileIndexing.boardY == NULL_INDEX 
        && board.InHitbox(mousePos)
    ) { 
        return CasePlayerTileInHandToBoard(selectedTileIndexing, mousePos);
    }

    // Case 4: Normal Player Tile on Board to Hand
    if(
        selectedTile
        && selectedTileIndexing.boardX != NULL_INDEX 
        && selectedTileIndexing.boardY != NULL_INDEX 
        && !board.InHitbox(mousePos)
    ) {         
        return CasePlayerTileOnBoardToHand(selectedTileIndexing, mousePos);
    }
    // Case 5: Normal Player Tile on Board to Another Place on Board
    if(
        selectedTile
        && selectedTileIndexing.boardX != NULL_INDEX 
        && selectedTileIndexing.boardY != NULL_INDEX 
        && board.InHitbox(mousePos)
    ) {       
        return CasePlayerTileOnBoardToElsewhereOnBoard(selectedTileIndexing, mousePos);
    }    

    selectedTile = nullptr;
    return false; // No tile movement occurred
}

bool TurnHandler::CasePlayerTileInHandToBoard(const TileIndex& selectedTileIndexing, const Vector2& mousePos) {
        Tile* targetBoardTilePtr = GetBoardTileUnderMousePointer(mousePos);
        // Check if we actually got a pointer
        if (!targetBoardTilePtr) {
           return false; 
        }
        Tile& targetBoardTile = *targetBoardTilePtr;
        Tile& selectedPlayerTile = *selectedTile;

        TileIndex boardIndexingInfo = targetBoardTile.GetTileIndexing();

        targetBoardTile.SetBoardTileDataToTempOccupiedState(
            selectedTileIndexing.handIndex
        );
        selectedPlayerTile.SetPlayerTileDataToTempPlacedState(
            boardIndexingInfo.boardX, 
            boardIndexingInfo.boardY, 
            targetBoardTile.GetOnScreenPosition(), 
            targetBoardTile.GetMultiplier(),
            targetBoardTile.GetColor(),
            targetBoardTile.GetType()
        );
        selectedTile = nullptr;
        return true; // Tile was moved from hand to board    
}

bool TurnHandler::CasePlayerTileOnBoardToHand(const TileIndex& selectedTileIndexing, const Vector2& mousePos) {
        Tile* boatdTilePtr = board.GetTilePointer(
            selectedTileIndexing.boardX, 
            selectedTileIndexing.boardY
        );
        
        // CRITICAL: Check for null pointer
        if (!boatdTilePtr) {
            selectedTile = nullptr;
            return false;
        }
        
        Tile& boardTile = *boatdTilePtr;
        
        // Validate tile state
        if (!boardTile.IsOccupied() || !boardTile.IsOccupiedBoardTileBecausePlayer()) {
            selectedTile = nullptr;
            return false;
        }
        
        Tile& selectedPlayerTile = *selectedTile;
                  
        boardTile.ResetTempOccupiedBoardTileData();
        selectedPlayerTile.ResetPlayerTileData();
        selectedTile = nullptr;
        return true; // Tile was moved from board to hand
}

bool TurnHandler::CasePlayerTileOnBoardToElsewhereOnBoard(const TileIndex& selectedTileIndexing, const Vector2& mousePos) {
        Tile* oldBoardTilePtr = board.GetTilePointer(
            selectedTileIndexing.boardX, 
            selectedTileIndexing.boardY
        );
        
        // CRITICAL: Check for null pointer
        if (!oldBoardTilePtr) {
            selectedTile = nullptr;
            return false;
        }
        
        Tile& oldBoardTile = *oldBoardTilePtr; // Convert back to reference for existing code
        
        Tile* newBoardTilePtr = GetBoardTileUnderMousePointer(mousePos);
        if (!newBoardTilePtr) {
            selectedTile = nullptr;
            return false;
        }
        Tile& newBoardTile = *newBoardTilePtr;
        
        Tile& selectedPlayerTile = *selectedTile;

        TileIndex newBoardIndexingInfo = newBoardTile.GetTileIndexing();

        oldBoardTile.ResetTempOccupiedBoardTileData();
        
        selectedPlayerTile.SetPlayerTileDataToTempPlacedState(
            newBoardIndexingInfo.boardX, 
            newBoardIndexingInfo.boardY, 
            newBoardTile.GetOnScreenPosition(), 
            newBoardTile.GetMultiplier(),
            newBoardTile.GetColor(),
            newBoardTile.GetType()
        );
        newBoardTile.SetBoardTileDataToTempOccupiedState(
            selectedTileIndexing.handIndex
        );        
        selectedTile = nullptr;
        return true;
}

// Turn update system
void TurnHandler::TurnUpdates() {
    UnapplyMultipliers();
    FindWordsInTurn();
    ApplyMultipliers();
    CalculatePlayerTurnScores();
    isTurnValid = !wordsInTurn.empty();
}

void TurnHandler::UnapplyMultipliers() {
    // Create a set to track tiles that have already been reset to avoid double-resetting
    std::unordered_set<Tile*> resetTiles;
    
    // Undo letter multipliers for all placed player tiles
    for (int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        if (currentTile.IsPlayerTilePlaced()) {
            int letterMultiplier = currentTile.GetMultiplier();
            if (letterMultiplier > 1) {
                currentTile.SetDisplayValue(currentTile.GetTilePointValue());
                resetTiles.insert(&currentTile);
            }
        }
    }

    // Undo word multipliers for all tiles in each word, but only if not already reset
    for (const TurnWord& wordInTurn : wordsInTurn) {
        if (wordInTurn.multiplier > 1) {
            for (Tile* tile : wordInTurn.tilesInWord) {
                // Only reset if not already reset by letter multiplier logic
                // Note: Committed board tiles can be reset here if they got word multipliers
                if (resetTiles.find(tile) == resetTiles.end()) {
                    tile->SetDisplayValue(tile->GetTilePointValue());
                    resetTiles.insert(tile);
                }
            }
        }
    }
}

void TurnHandler::ApplyMultipliers() {
    // Apply letter multipliers for all placed player tiles
    for (int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        if (currentTile.IsPlayerTilePlaced()) {
            int letterMultiplier = currentTile.GetMultiplier();
            if (letterMultiplier > 1) {
                currentTile.DoMultiplier(letterMultiplier);
            }
        }
    }
    // Apply word multipliers for all tiles in each word, but only once per tile
    std::unordered_set<Tile*> wordMultipliedTiles;
    for (const TurnWord& wordInTurn : wordsInTurn) {
        if (wordInTurn.multiplier > 1) {
            for (Tile* tile : wordInTurn.tilesInWord) {
                // Apply word multipliers to ALL tiles in the word (including committed board tiles)
                // Only apply if not already applied for this tile
                if (wordMultipliedTiles.find(tile) == wordMultipliedTiles.end()) {
                    tile->DoMultiplier(wordInTurn.multiplier);
                    wordMultipliedTiles.insert(tile);
                }
            }
        }
    }
}

void TurnHandler::CalculatePlayerTurnScores() {
    currentPlayerTurnScore = 0;

    for (TurnWord& wordInTurn : wordsInTurn) {
        wordInTurn.wordValue = 0;
        for (const Tile* tile : wordInTurn.tilesInWord) {
            wordInTurn.wordValue += tile->GetDisplayPointValue();
        }
        currentPlayerTurnScore += wordInTurn.wordValue;       
    }
}

// Word finding and validation

void TurnHandler::FindWordsInTurn() {
    std::vector<TurnWord> updateVector;

    PlayerWordIndexesAndDir playerWordInfo = FindPlayerWordInfo();
    TurnWord playerWord;

    // Only add a word if it's valid
    if (playerWordInfo.wordColIndex != NULL_INDEX && playerWordInfo.wordRowIndex != NULL_INDEX) {
        playerWord = MakePlayerWord(playerWordInfo);
    }

    bool allPlacedTilesInPlayerWord = AllPlacedTilesInPlayerWord(playerWord);

    if (!playerWord.tilesInWord.empty() && allPlacedTilesInPlayerWord) {
        updateVector.push_back(playerWord);
    }    
    if (!playerWord.tilesInWord.empty() && allPlacedTilesInPlayerWord) {
        MakeBranchWords(playerWord, playerWordInfo, updateVector);
    }

    if(AreTurnWordsPlacedValid(updateVector)) {
        wordsInTurn = updateVector;
    } else {
        wordsInTurn.clear();
    }
}

bool TurnHandler::AllPlacedTilesInPlayerWord(const TurnWord& playerWord) {
    // If a placed tile isn't in the word then the turn is invalid
    for (int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        if (currentTile.IsPlayerTilePlaced()) {
            bool found = false;
            for (size_t j = 0; j < playerWord.tilesInWord.size(); j++) {
                if (playerWord.tilesInWord[j] == &currentTile) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}

PlayerWordIndexesAndDir TurnHandler::FindPlayerWordInfo() {
    PlayerWordIndexesAndDir playerWordInfo{};
    const PlayerWordIndexesAndDir emptyInfo{};

    struct IntVector2 {
        int x = NULL_INDEX;
        int y = NULL_INDEX;
    };

    IntVector2 topLeftMostPlacedTile;
    int amountsOfTilesPlaced = 0;

    // Finding top left tile and amount of placed tiles
    for (int i = 0; i < PLAYER_HAND_SIZE; i++) {
        Tile* currentTilePtr = players[currentPlayerIndex].GetTilePointer(i);
        if (!currentTilePtr) continue;
        Tile& currentTile = *currentTilePtr;
        if (currentTile.IsPlayerTilePlaced()) {
            TileIndex currentTileIndex = currentTile.GetTileIndexing();
            if (topLeftMostPlacedTile.x == NULL_INDEX && topLeftMostPlacedTile.y == NULL_INDEX) {
                topLeftMostPlacedTile.x = currentTileIndex.boardX;
                topLeftMostPlacedTile.y = currentTileIndex.boardY;
            }
            if (currentTileIndex.boardX < topLeftMostPlacedTile.x) {
                topLeftMostPlacedTile.x = currentTileIndex.boardX;
            }
            if (currentTileIndex.boardY < topLeftMostPlacedTile.y) {
                topLeftMostPlacedTile.y = currentTileIndex.boardY;
            }
            amountsOfTilesPlaced++;
        }
    }
    if (amountsOfTilesPlaced == 0) {
        return emptyInfo;
    } 

    // Finding if a 'player word' exist downward and to the right
    bool toDown = CheckForPlayerWordDown(topLeftMostPlacedTile.x, topLeftMostPlacedTile.y);
    bool toRight = CheckForPlayerWordToRight(topLeftMostPlacedTile.x, topLeftMostPlacedTile.y);

    // If no words found in either direction, turn is invalid
    if (!toRight && !toDown) {
        return emptyInfo;
    }

    // If both directions have words, choose the one with more player tiles
    if (toRight && toDown) {
        int playerTilesHorizontal = CountPlayerTilesInDirection(topLeftMostPlacedTile.x, topLeftMostPlacedTile.y, true);
        int playerTilesVertical = CountPlayerTilesInDirection(topLeftMostPlacedTile.x, topLeftMostPlacedTile.y, false);
        
        if (playerTilesVertical > playerTilesHorizontal) {
            toRight = false; // Choose vertical
        }
        // else: keep toRight true (horizontal), do not set toDown = false as it's not used after this
    }  

    // Updating playerWordInfo correctly
    if(toRight) {
        playerWordInfo.isHorizontal = true;
        playerWordInfo.wordColIndex = topLeftMostPlacedTile.x;
        playerWordInfo.wordRowIndex = topLeftMostPlacedTile.y;
    } else {
        playerWordInfo.isHorizontal = false;
        playerWordInfo.wordColIndex = topLeftMostPlacedTile.x;
        playerWordInfo.wordRowIndex = topLeftMostPlacedTile.y;        
    }

    return playerWordInfo;
}

bool TurnHandler::CheckForPlayerWordToRight(int x, int y) {
    int runningTotal = 0;
    int playerTiles = 0;

    // Find the actual start of the word by going left until we hit an empty tile
    int startX = x;
    while (startX > 0) {
        const Tile* prevTile = board.GetTilePointer(startX - 1, y);
        if (!prevTile || !prevTile->IsOccupied()) {
            break;
        }
        startX--;
    }

    // Finding if a string of tiles exist, such that there is more than two tiles and at least one playerTile
    for (int i = startX; i < BOARD_WIDTH; i++) {
        const Tile* currentBoardTile = board.GetTilePointer(i, y);
        if (!currentBoardTile) {
            break;
        }
        if (currentBoardTile->IsOccupied()) {
            runningTotal++;
            if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
                playerTiles++;
            }
        } else {
            break;
        }
    }
    if (runningTotal >= WORD_LETTER_REQUIREMENT && playerTiles >= 1) {
        return true;
    }
    return false;
}

bool TurnHandler::CheckForPlayerWordDown(int x, int y) {
    int runningTotal = 0;
    int playerTiles = 0;

    // Find the actual start of the word by going up until we hit an empty tile
    int startY = y;
    while (startY > 0) {
        const Tile* prevTile = board.GetTilePointer(x, startY - 1);
        if (!prevTile || !prevTile->IsOccupied()) {
            break;
        }
        startY--;
    }

    // Finding if a string of tiles exist, such that there is more than two tiles and at least one playerTile
    for (int i = startY; i < BOARD_HEIGHT; i++) {
        const Tile* currentBoardTile = board.GetTilePointer(x, i);
        if (!currentBoardTile) {
            break;
        }
        if (currentBoardTile->IsOccupied()) {
            runningTotal++;
            if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
                playerTiles++;
            }
        } else {
            break;
        }
    }
    if (runningTotal >= WORD_LETTER_REQUIREMENT && playerTiles >= 1) {
        return true;
    }
    return false;
}

int TurnHandler::CountPlayerTilesInDirection(int x, int y, bool isHorizontal) {
    int playerTiles = 0;
    
    if (isHorizontal) {
        // Find the actual start of the word by going left
        int startX = x;
        while (startX > 0) {
            const Tile* prevTile = board.GetTilePointer(startX - 1, y);
            if (!prevTile || !prevTile->IsOccupied()) {
                break;
            }
            startX--;
        }
        
        // Count player tiles going right from start
        for (int i = startX; i < BOARD_WIDTH; i++) {
            const Tile* currentBoardTile = board.GetTilePointer(i, y);
            if (!currentBoardTile) {
                break;
            }
            if (currentBoardTile->IsOccupied()) {
                if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
                    playerTiles++;
                }
            } else {
                break;
            }
        }
    } else {
        // Find the actual start of the word by going up
        int startY = y;
        while (startY > 0) {
            const Tile* prevTile = board.GetTilePointer(x, startY - 1);
            if (!prevTile || !prevTile->IsOccupied()) {
                break;
            }
            startY--;
        }
        
        // Count player tiles going down from start
        for (int i = startY; i < BOARD_HEIGHT; i++) {
            const Tile* currentBoardTile = board.GetTilePointer(x, i);
            if (!currentBoardTile) {
                break;
            }
            if (currentBoardTile->IsOccupied()) {
                if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
                    playerTiles++;
                }
            } else {
                break;
            }
        }
    }
    
    return playerTiles;
}

TurnWord TurnHandler::MakePlayerWord(PlayerWordIndexesAndDir playerWordInfo) {
    TurnWord playerWord;
    if (playerWordInfo.isHorizontal) {
        playerWord = MakeWordToRight(playerWordInfo.wordColIndex, playerWordInfo.wordRowIndex);
    } else {
        playerWord = MakeWordDown(playerWordInfo.wordColIndex, playerWordInfo.wordRowIndex);
    }
    // Only return if the word is valid (has tiles and a string)
    if (!playerWord.tilesInWord.empty() && !playerWord.wordString.empty()) {
        return playerWord;
    }
    return TurnWord{};
}

void TurnHandler::MakeBranchWords(TurnWord playerWord, PlayerWordIndexesAndDir playerWordInfo, std::vector<TurnWord>& updateVector) {
    // For each player tile in the main word, check for branching words in the perpendicular direction
    for (size_t i = 0; i < playerWord.tilesInWord.size(); i++) {
        Tile* playerTile = playerWord.tilesInWord[i];
        // Only check for branching words if this tile was placed by the player this turn
        if (playerTile->IsPlayerTilePlaced()) {
            PlayerWordIndexesAndDir branchingWordAttemptInfo;
            branchingWordAttemptInfo.isHorizontal = !playerWordInfo.isHorizontal;
            branchingWordAttemptInfo.wordColIndex = playerTile->GetTileIndexing().boardX;
            branchingWordAttemptInfo.wordRowIndex = playerTile->GetTileIndexing().boardY;

            TurnWord branchWord = MakePlayerWord(branchingWordAttemptInfo);
            // Only add if it's a valid word (not empty, not just the single tile)
            if (!branchWord.tilesInWord.empty() && branchWord.tilesInWord.size() > 1) {
                updateVector.push_back(branchWord);
            }
        }
    }  
}

TurnWord TurnHandler::MakeWordToRight(int x, int y) {
    TurnWord word;
    int runningTotal = 0;
    int playerTilesTotal = 0;
    
    // Find the actual start of the word by going left until we hit an empty tile
    int startX = x;
    while (startX > 0) {
        Tile* prevTile = board.GetTilePointer(startX - 1, y);
        if (!prevTile || !prevTile->IsOccupied()) {
            break;
        }
        startX--;
    }

    // Scan from the actual start of the word to the right
    for (int i = startX; i < BOARD_WIDTH; i++) {
        Tile* currentBoardTile = board.GetTilePointer(i, y);
        
        // CRITICAL: Check for null pointer (invalid coordinates)
        if (!currentBoardTile) {
            break; // Hit invalid coordinates, stop word detection
        }
        
        Tile* currentPlayerTile = nullptr;
        if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
            currentPlayerTile = players[currentPlayerIndex].GetTilePointer(currentBoardTile->GetHandIndex());
        }

        if (currentBoardTile->IsOccupied()) {
            runningTotal++;
            if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
                playerTilesTotal++;
                if (currentBoardTile->GetType() == DOUBL_WORD || currentBoardTile->GetType() == TRIPL_WORD) {
                    word.multiplier += currentBoardTile->GetMultiplier() - 1;
                    // This is used for coloring, TRIPL_WORD_TYPE takes priority over DOUBL_WORD_TYPE
                    if(currentBoardTile->GetType() == DOUBL_WORD && word.wordMultType != TRIPL_WORD_TYPE) {
                        word.wordMultType = DOUBL_WORD_TYPE;
                    }
                    if(currentBoardTile->GetType() == TRIPL_WORD) {
                        word.wordMultType = TRIPL_WORD_TYPE;
                    }
                }
                word.tilesInWord.push_back(currentPlayerTile);
                word.wordString += currentPlayerTile->GetLetter();
            } else {
                word.tilesInWord.push_back(currentBoardTile);
                word.wordString += currentBoardTile->GetLetter();
            }
        } else {
            // End of word
            break;
        }
    }
    
    // Check if we have a valid word
    if (runningTotal >= 2 && playerTilesTotal >= 1) {
        return word;
    }
    return TurnWord{};
}

TurnWord TurnHandler::MakeWordDown(int x, int y) {
    TurnWord word;
    int runningTotal = 0;
    int playerTilesTotal = 0;
    
    // Find the actual start of the word by going up until we hit an empty tile
    int startY = y;
    while (startY > 0) {
        Tile* prevTile = board.GetTilePointer(x, startY - 1);
        if (!prevTile || !prevTile->IsOccupied()) {
            break;
        }
        startY--;
    }

    // Scan from the actual start of the word downward
    for (int i = startY; i < BOARD_HEIGHT; i++) {
        Tile* currentBoardTile = board.GetTilePointer(x, i);
        
        // CRITICAL: Check for null pointer (invalid coordinates)
        if (!currentBoardTile) {
            break; // Hit invalid coordinates, stop word detection
        }
        
        Tile* currentPlayerTile = nullptr;
        if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
            currentPlayerTile = players[currentPlayerIndex].GetTilePointer(currentBoardTile->GetHandIndex());
        }

        if (currentBoardTile->IsOccupied()) {
            runningTotal++;
            if (currentBoardTile->IsOccupiedBoardTileBecausePlayer()) {
                playerTilesTotal++;
                if (currentBoardTile->GetType() == DOUBL_WORD || currentBoardTile->GetType() == TRIPL_WORD) {
                    word.multiplier += currentBoardTile->GetMultiplier() - 1;
                    // This is used for coloring, TRIPL_WORD_TYPE takes priority over DOUBL_WORD_TYPE
                    if(currentBoardTile->GetType() == DOUBL_WORD && word.wordMultType != TRIPL_WORD_TYPE) {
                        word.wordMultType = DOUBL_WORD_TYPE;
                    }
                    if(currentBoardTile->GetType() == TRIPL_WORD) {
                        word.wordMultType = TRIPL_WORD_TYPE;
                    }
                }
                word.tilesInWord.push_back(currentPlayerTile);
                word.wordString += currentPlayerTile->GetLetter();
            } else {
                word.tilesInWord.push_back(currentBoardTile);
                word.wordString += currentBoardTile->GetLetter();
            }
        } else {
            // End of word
            break;
        }
    }
    
    // Check if we have a valid word
    if (runningTotal >= 2 && playerTilesTotal >= 1) {
        return word;
    }
    return TurnWord{};
}

bool TurnHandler::AreTurnWordsPlacedValid(const std::vector<TurnWord>& updateVector) {
    // No need for this check, a committedBoardTile wont even exist
    if(currentTurn == 1) {
        TileIndex startTileIndexing = board.GetStartTileIndexing();
        
        for(const TurnWord& wordInTurn : updateVector) {
            for(const Tile* tile : wordInTurn.tilesInWord) {
                TileIndex currentTileIndexing = tile->GetTileIndexing();
                if(    (currentTileIndexing.boardX == startTileIndexing.boardX) 
                    && (currentTileIndexing.boardY == startTileIndexing.boardY)
                ) {
                    return true;
                }
            }
        }
        
        return false;
    }

    // If we find a singular comittedBoardTile in any of the turnWords the turn is valid
    for(const TurnWord& wordInTurn : updateVector) {
        for(const Tile* tile : wordInTurn.tilesInWord) {
            if(tile->IsCommittedBoardTile()) {
                return true;
            }
        }  
    }
    
    return false;  
}
