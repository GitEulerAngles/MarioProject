#include "game.h"
#include "physics.h"

void setup() {
    FreeImage_Initialise(FALSE);
    blockTypes = initVector(9900);
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
    backgroundTexture = createSprite("C:/Users/Kurt/source/repos/MarioProject1/MarioProject1/backgroundTextures.png");
}
void input() {
    if (grounded) {
        vel.x = floorf(vel.x * 10) / 10;
        if (running) {
            if (vel.x < 15 && vel.x > 0)
                vel.x += 0.3;
            else if (vel.x > -15 && vel.x < 0)
                vel.x -= 0.3;
        }
        if (vel.x > 0)
            vel.x -= 0.1;
        else if (vel.x < 0)
            vel.x += 0.1;
    }

    printf("%d\n", running);

    dynamicSprites[0].pos.x += vel.x;
    dynamicSprites[0].pos.y += vel.y;
    applyGravity(grounded, &vel);

    physicsPos = dynamicBoxes[0];
}
void update() {
    grounded = false;
    struct Vector2f offset = { 10,25 };
    dynamicBoxes[0] = createBounds(dynamicSprites[0].pos, dynamicSprites[0].dim, offset);

    updateBlocks();

    struct Vector2f blockoffset = { 0,0 };
    for (int i = 0; i < 121; i++)
        staticBoxes[i] = createBounds(staticSprites[i].pos, staticSprites[i].dim, blockoffset);

    for (int16_t i = 0; i < 121; i++)
    if (getElement(&blockTypes, spritePosition(i)) != AIR) {
        struct Vector2f collision = resolveCollision(dynamicBoxes[0], staticBoxes[i], physicsPos, &grounded);
            
        if (collision.y > 0)
            grounded = true;

        else if (collision.y < 0)
            vel.y = 0;

        dynamicSprites[0].pos.x -= collision.x;
        dynamicSprites[0].pos.y -= collision.y;
        dynamicBoxes[0] = createBounds(dynamicSprites[0].pos, dynamicSprites[0].dim, offset);
    }

    updateCamera(dynamicSprites[0].pos);
    updateBackground(cameraPos);
}
void render() {
    static int count, delay = 0;

    delay++;

    if (delay > 5) {
        delay = 0;
        count++;
        if (count > 2)
            count = 0;
        for (int16_t i = 0; i < 169; i++)
        if (getElement(&blockTypes, spritePosition(i)) == QUESTION) {
            staticSprites[i].imageCoordinate.x = count * 16;
        }
    }

    drawPlayer(&animation);
}
void clean() {
    FreeImage_Unload(marioTexture);
    FreeImage_DeInitialise();
    PostQuitMessage(0);
}
