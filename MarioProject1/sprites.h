#ifndef SPRITES
#define SPRITES

#include "math.h"

enum animationType {IDLE, WALKING};

struct playerAnimation {
    enum animationType type;
    int frame;
    int delay;
};

struct sprite {
    // Bitmaps
    FIBITMAP* bitmap;
    FIBITMAP* resizedBitmap;
    // Image coordinates
    struct Vector2i imageCoordinate;
    struct Vector2i imageDimension;
    // Image dimensions
    struct Vector2i originalDim;
    struct Vector2i newDim;

    struct Vector2i vel;
    struct Vector2i pos;
};

struct sprite sprites[1];
struct playerAnimation animation;

struct playerAnimation createAnimation();

void drawPlayer();
void createSprite(int index, char path[]);

#endif // SPRITES
