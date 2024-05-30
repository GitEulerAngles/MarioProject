#ifndef SPRITES
#define SPRITES

#include "math.h"
#include "player.h"
#define MULTIPLIER 10

enum BlockType {AIR, QUESTION, GRASS};

struct Sprite {
    // Image coordinates
    struct Vector2i imageCoordinate;
    struct Vector2i imageDimension;
    // On screen data
    struct Vector2f pos;
    struct Vector2i dim;
};

FIBITMAP* marioTexture;
FIBITMAP* blockTexture;
FIBITMAP* backgroundTexture;

struct Sprite backgroundSprite[2];
struct Sprite dynamicSprites[1];
struct Sprite staticSprites[121];

struct Vector blockTypes;

int spritePosition(int i);
void updateBackground(struct Vector2f pos);
void drawPlayer(struct playerAnimation* p);
void updateBlocks();
FIBITMAP* createSprite(char path[]);

#endif // SPRITES
