#include "game.h"

void setup() {
    FreeImage_Initialise(FALSE);
    sprites[0].imageCoordinate.x = 16;
    animation = createAnimation();
    createSprite(0, "C:/Users/Kurt/source/repos/MarioProject1/MarioProject1/marioTextures.png");
}
void input() {
}
void update() {
    sprites[0].pos.x += sprites[0].vel.x*5;

    drawPlayer(&animation);
}
void render() {
    
}
void clean() {
    for (int i = 0; i < 1; i++) {
        FreeImage_Unload(sprites[i].bitmap);
        FreeImage_Unload(sprites[i].resizedBitmap);
    }
    FreeImage_DeInitialise();
    PostQuitMessage(0);
}