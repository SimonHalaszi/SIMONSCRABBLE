#include "player.hpp"

Player::Player() {
    sharedTilePool = nullptr;
    for(int i = 0; i < PLAYER_HAND_SIZE; i++) {
        playerHand[i] = Tile(
            {NULL_POSITION, NULL_POSITION},
            PLAYER,
            DNE,
            0,
            '\0',
            {NULL_INDEX, NULL_INDEX, i},
            TILE_TYPE_MULTIPLIERS[DNE]
        );
    }
    playerScore = 0;

    hitBox = { 
        HAND_START_X, 
        HAND_START_Y, 
        HAND_DISPLAY_LENGTH, 
        TILE_SIZE 
    };    

    outlineRectangle = {
        HAND_START_X - UI_OUTLINE_THICKNESS,
        HAND_START_Y - UI_OUTLINE_THICKNESS,
        HAND_DISPLAY_LENGTH + (UI_OUTLINE_THICKNESS * 2),
        TILE_SIZE + (UI_OUTLINE_THICKNESS * 2)
    };
}

Player::Player(TilePool* tilePool) {
    hitBox = { 
        HAND_START_X, 
        HAND_START_Y, 
        HAND_DISPLAY_LENGTH, 
        TILE_SIZE 
    };      
    sharedTilePool = tilePool; 
    for(int i = 0; i < PLAYER_HAND_SIZE; i++) {
        playerHand[i] = Tile(
            {NULL_POSITION, NULL_POSITION},
            PLAYER,
            DNE,
            0,
            '\0',
            {NULL_INDEX, NULL_INDEX, i},
            TILE_TYPE_MULTIPLIERS[DNE]
        );
    }
    playerScore = 0;

    outlineRectangle = {
        HAND_START_X - UI_OUTLINE_THICKNESS,
        HAND_START_Y - UI_OUTLINE_THICKNESS,
        HAND_DISPLAY_LENGTH + (UI_OUTLINE_THICKNESS * 2),
        TILE_SIZE + (UI_OUTLINE_THICKNESS * 2)
    };    
}

void Player::Draw() const {
    DrawRectangleRec(outlineRectangle, OUTLINE_COLOR);
    DrawHand();
}

// For both filling and refilling the player's hand
void Player::FillHand() {
    int vowelCount = 0;
    int tilesToFill = 0;
    // First, count how many tiles need to be filled and how many vowels are already present
    for (int i = 0; i < PLAYER_HAND_SIZE; i++) {
        if (!playerHand[i].IsDNE()) {
            // Check if the tile is a vowel
            char letter = playerHand[i].GetLetter();
            for (int v = 0; v < VOWELS_COUNT; v++) {
                if (letter == LETTER_INDEXES[VOWELS_INDEXES[v]]) {
                    vowelCount++;
                    break;
                }
            }
        } else {
            tilesToFill++;
        }
    }

    int vowelsNeeded = VOWEL_TRY_COUNT - vowelCount;
    for (int i = 0; i < PLAYER_HAND_SIZE; i++) {
        if(playerHand[i].IsDNE()) {
            Vector2 onScreenPosition = {HAND_START_X + (i * (TILE_SIZE + TILE_GAP)), HAND_START_Y};

            if (!sharedTilePool->LettersAvailable()) {
                playerHand[i] = Tile(
                    {onScreenPosition.x, onScreenPosition.y},
                    PLAYER,
                    DNE,
                    0,
                    '\0',
                    {NULL_INDEX, NULL_INDEX, i},
                    TILE_TYPE_MULTIPLIERS[DNE]
                );
                continue;
            }

            int randomIndex = 0;
            // If we still need vowels, and vowels are available, force a vowel
            if (vowelsNeeded > 0 && sharedTilePool->AreVowelsAvailable()) {
                do {
                    int randomVowelIndex = GetRandomValue(0, VOWELS_COUNT - 1);
                    randomIndex = VOWELS_INDEXES[randomVowelIndex];
                } while (!sharedTilePool->IsLetterAvailable(randomIndex));
                vowelsNeeded--;
            } else {
                // Pick any available letter
                do {
                    randomIndex = GetRandomValue(0, TOTAL_LETTERS - 1);
                } while (!sharedTilePool->IsLetterAvailable(randomIndex));
            }

            // Create a new tile with the random letter
            if (randomIndex == MYSTERY_TILE_INDEX) {
                playerHand[i] = Tile(
                    {onScreenPosition.x, onScreenPosition.y},
                    PLAYER,
                    MYSTERY,
                    LETTER_SCORE_VALUES[randomIndex],
                    LETTER_INDEXES[randomIndex],
                    {NULL_INDEX, NULL_INDEX, i},
                    TILE_TYPE_MULTIPLIERS[MYSTERY]
                );
            } else {
                playerHand[i] = Tile(
                    {onScreenPosition.x, onScreenPosition.y},
                    PLAYER,
                    NORMAL,
                    LETTER_SCORE_VALUES[randomIndex],
                    LETTER_INDEXES[randomIndex],
                    {NULL_INDEX, NULL_INDEX, i},
                    TILE_TYPE_MULTIPLIERS[NORMAL]
                );
            }

            sharedTilePool->TakeLetter(randomIndex);
        }
    }
}

void Player::DrawHand() const {
    for(int i = 0; i < PLAYER_HAND_SIZE; i++) {
        if (playerHand[i].IsPlayerTilePlaced()) {
            DrawRectangle(
                playerHand[i].GetInHandPosition().x, 
                playerHand[i].GetInHandPosition().y, 
                TILE_SIZE, 
                TILE_SIZE, 
                DNE_TILE_COLOR
            );
        }
        playerHand[i].Draw();
    }
}

Tile* Player::GetTilePointer(int tileIndex) {
    if (tileIndex < 0 || tileIndex >= PLAYER_HAND_SIZE) {
        return nullptr;
    }
    return &playerHand[tileIndex];     
}
