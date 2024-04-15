#include "game.h"
#include "physics.h"

void setup() {
    FreeImage_Initialise(FALSE);
    dynamicSprites[0].imageCoordinate.x = 16;
    dynamicSprites[0].imageCoordinate.y = 16;
    dynamicSprites[0].imageDimension.x = 16;
    dynamicSprites[0].imageDimension.y = 20;
    dynamicSprites[0].dim.x = 10 * MULTIPLIER;
    dynamicSprites[0].dim.y = 12.5 * MULTIPLIER;
    dynamicSprites[0].pos.x = 0;
    dynamicSprites[0].pos.y = 0;

    animation = createAnimation();
    createSprite(dynamicSprites, 0, "C:/Users/Kurt/source/repos/MarioProject1/MarioProject1/marioTextures.png");
    for (int i = 0; i < 100; i++) {
        struct Vector2i coords = intToVector(i);

        createSprite(staticSprites, i, "C:/Users/Kurt/source/repos/MarioProject1/MarioProject1/blockTextures.png");
        staticSprites[i].imageDimension.x = 16;
        staticSprites[i].imageDimension.y = 16;
        staticSprites[i].imageCoordinate.x = 0;
        staticSprites[i].imageCoordinate.y = 0;
        staticSprites[i].pos.x = coords.x * 10 * MULTIPLIER;
        staticSprites[i].pos.y = coords.y * 10 * MULTIPLIER;
        staticSprites[i].dim.x = 10 * MULTIPLIER;
        staticSprites[i].dim.y = 10 * MULTIPLIER;
    }
}
void input() {

}
void update() {
    dynamicSprites[0].pos.x += vel.x * 5;
    dynamicSprites[0].pos.y += vel.y * 5;

    boxes[0] = createBounds(dynamicSprites[0].pos, dynamicSprites[0].dim);



    drawPlayer(&animation);
    drawBlocks();
}
void render() {

}
void clean() {
    for (int i = 0; i < 1; i++) {
        FreeImage_Unload(dynamicSprites[i].bitmap);
        FreeImage_Unload(dynamicSprites[i].resizedBitmap);
    }
    FreeImage_DeInitialise();
    PostQuitMessage(0);
}