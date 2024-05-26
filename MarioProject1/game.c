#include "game.h"
#include "physics.h"

void setup() {
    FreeImage_Initialise(FALSE);
    dynamicSprites[0].imageCoordinate.x = 16;
    dynamicSprites[0].imageCoordinate.y = 0;
    dynamicSprites[0].imageDimension.x = 16;
    dynamicSprites[0].imageDimension.y = 23;
    dynamicSprites[0].dim.x = 10 * MULTIPLIER;
    dynamicSprites[0].dim.y = 12.5 * MULTIPLIER;
    dynamicSprites[0].pos.x = 0;
    dynamicSprites[0].pos.y = 0;

    animation = createAnimation();

    createMap();

    updateBlocks();

    marioTexture = createSprite("C:/Users/Kurt/source/repos/MarioProject1/MarioProject1/marioTextures.png");
    blockTexture = createSprite("C:/Users/Kurt/source/repos/MarioProject1/MarioProject1/blockTextures.png");

    for (int i = 0; i < 100; i++)
        staticBoxes[i] = createBounds(staticSprites[i].pos, staticSprites[i].dim);
}
void input() {
    dynamicSprites[0].pos.x += vel.x;
    dynamicSprites[0].pos.y += vel.y;
    applyGravity(grounded, &vel);
}
void update() {
    grounded = false;
    dynamicBoxes[0] = createBounds(dynamicSprites[0].pos, dynamicSprites[0].dim);

    for (int i = 0; i < 100; i++)
    if (blockTypes[i] != AIR) {
        struct Vector2f collision = resolveCollision(dynamicBoxes[0], staticBoxes[i], false, &grounded);
            
        if (collision.y > 0) {
            grounded = true;
        }
        else if (collision.y < 0) {
            vel.y = 0;
        }

        dynamicSprites[0].pos.x -= collision.x;
        dynamicSprites[0].pos.y -= collision.y;
        dynamicBoxes[0] = createBounds(dynamicSprites[0].pos, dynamicSprites[0].dim);
    }
}
void render() {
    static int delay = 0;

    delay++;

    if (delay > 5) {
        delay = 0;
        for (int i = 0; i < 100; i++) {
            if (blockTypes[i] == QUESTION) {
                staticSprites[i].imageCoordinate.x += 16;
                if (staticSprites[i].imageCoordinate.x > 32) {
                    staticSprites[i].imageCoordinate.x = 0;
                }
            }
        }
    }

    updateBlocks();
    drawPlayer(&animation);
}
void clean() {
    FreeImage_Unload(marioTexture);
    FreeImage_DeInitialise();
    PostQuitMessage(0);
}
