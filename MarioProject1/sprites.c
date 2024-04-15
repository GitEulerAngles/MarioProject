#include "window.h"

struct playerAnimation createAnimation() {
    struct playerAnimation newP;
    newP.type = IDLE;
    newP.frame = 0;
    newP.delay = 16;
    return newP;
}

void createSprite(struct sprite *sprites, int index, char path[]) {
    struct sprite* current_sprite = &sprites[index];

    current_sprite->bitmap = loadPNGImage(path);
    current_sprite->originalDim.x = FreeImage_GetWidth(current_sprite->bitmap);
    current_sprite->originalDim.y = FreeImage_GetHeight(current_sprite->bitmap);
    current_sprite->newDim.x = (current_sprite->originalDim.x) * MULTIPLIER;
    current_sprite->newDim.y = (current_sprite->originalDim.y) * MULTIPLIER;
    current_sprite->resizedBitmap = FreeImage_Rescale(current_sprite->bitmap, current_sprite->newDim.x, current_sprite->newDim.y, FILTER_BOX);
}

void drawBlocks() {

}

void drawPlayer(struct playerAnimation * p) {
    if (p->type == IDLE)
        p->frame = 0;

    else {
        p->delay += 1;
        if (p->delay > 8) {
            p->delay = 0;
            if (p->frame == 0)
                p->frame = 1;
            else
                p->frame = 0;
        }
    }

    if (p->frame == 0)
        dynamicSprites[0].imageCoordinate.x = 0;
    if (p->frame == 1)
        dynamicSprites[0].imageCoordinate.x = 16;
}
