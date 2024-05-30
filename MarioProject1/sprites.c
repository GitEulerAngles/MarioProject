#include "window.h"
#include <stdio.h>

const int length = 90;

void updateBackground(struct Vector2f pos) {
    for (int i = 0; i < 2; i++) {
        backgroundSprite[i].imageCoordinate.x = 0;
        backgroundSprite[i].imageCoordinate.y = 0;
        backgroundSprite[i].imageDimension.x = 128;
        backgroundSprite[i].imageDimension.y = 108;
        backgroundSprite[i].dim.x = 1016;
        backgroundSprite[i].dim.y = 1039;
        backgroundSprite[i].pos.y = pos.y;

        float x = fmod(pos.x, 10000) * 0.0001;
        float bpos = x * 1016;

        backgroundSprite[i].pos.x = pos.x - bpos + 1016 * i;
    }
}

int spritePosition(int i) {
    struct Vector2i coords = intToVector(i, 11);

    int blockX = floor(cameraPos.x / 100) + coords.x;
    int blockY = floor(cameraPos.y / 100) + coords.y;

    if (blockX < 0 || blockY < 0 ||
        blockX >= length || blockY >= length)
        return 0;

    return vectorToInt(blockX, blockY, length);
}

FIBITMAP* createSprite(char path[]) {
    FIBITMAP* map = loadPNGImage(path);

    int w = FreeImage_GetWidth(map);
    int h = FreeImage_GetHeight(map);

    FIBITMAP* resizedMap = FreeImage_Rescale(map, w * MULTIPLIER, h * MULTIPLIER, FILTER_BOX);
    FreeImage_Unload(map);
    return resizedMap;
}

void createMap() {
    FILE* file;
    errno_t err = fopen_s(&file, "map.txt", "r");

    if (err != 0) {
        perror("Failed to open file");
        return 1;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch >= '0' && ch <= '9') {
            char str[2] = { ch, '\0' };
            addElement(&blockTypes, atoi(str));
        }
    }

    fclose(file);
}

void updateBlocks() {
    for (int i = 0; i < 121; i++) {
        struct Vector2i coords = intToVector(i, 11);

        int blockX = floor(cameraPos.x / 100) + coords.x;
        int blockY = floor(cameraPos.y / 100) + coords.y;

        if (blockX < 0 || blockY < 0 ||
            blockX >= length || blockY >= length)
            continue;

        int block = vectorToInt(blockX, blockY, length);

        if (getElement(&blockTypes, block) == QUESTION) {
            staticSprites[i].imageDimension.x = 16;
            staticSprites[i].imageDimension.y = 16;
            staticSprites[i].imageCoordinate.y = 0;
            staticSprites[i].pos.x = blockX * 10 * MULTIPLIER;
            staticSprites[i].pos.y = blockY * 10 * MULTIPLIER;
            staticSprites[i].dim.x = 10 * MULTIPLIER;
            staticSprites[i].dim.y = 10 * MULTIPLIER;
        }
        else if (getElement(&blockTypes, block) == GRASS) {
            staticSprites[i].imageDimension.x = 16;
            staticSprites[i].imageDimension.y = 16;

            int neighbors[4][2] = { {0,-1},{1,0},{0,1},{-1,0} };
            int count = 0;

            // Get neighboring squares
            for (int i = 0; i < 4; i++) {
                struct Vector2i new_coord = { blockX + neighbors[i][0], blockY + neighbors[i][1] };

                if (new_coord.x < 0 || new_coord.y < 0 || 
                    new_coord.x >= length || new_coord.y >= length)
                    continue;

                if (getElement(&blockTypes, vectorToInt(new_coord.x, new_coord.y, length)) == GRASS)
                    count += pow(2, i);
            }

            struct Vector2i imageCoords = intToVector(count, 3);

            staticSprites[i].imageCoordinate.x = imageCoords.x * 16;
            staticSprites[i].imageCoordinate.y = (imageCoords.y + 1) * 16;
            staticSprites[i].pos.x = blockX * 10 * MULTIPLIER;
            staticSprites[i].pos.y = blockY * 10 * MULTIPLIER;
            staticSprites[i].dim.x = 10 * MULTIPLIER;
            staticSprites[i].dim.y = 10 * MULTIPLIER;
        }
        else {
            staticSprites[i].imageDimension.x = 0;
            staticSprites[i].imageDimension.y = 0;
            staticSprites[i].pos.x = blockX * 10 * MULTIPLIER;
            staticSprites[i].pos.y = blockY * 10 * MULTIPLIER;
            staticSprites[i].dim.x = 10 * MULTIPLIER;
            staticSprites[i].dim.y = 10 * MULTIPLIER;
        }
    }
}

void drawPlayer(struct playerAnimation * p) {
    if (p->type == IDLE)
        dynamicSprites[0].imageCoordinate.x = 0;
    else if (p->type == JUMPING)
        dynamicSprites[0].imageCoordinate.x = 32;
    else if (p->type == FALLING)
        dynamicSprites[0].imageCoordinate.x = 48;
    else {
        p->delay += 1;
        if (p->delay > 10 - abs(vel.x / 3)) {
            p->delay = 0;
            if (p->frame == 0)
                p->frame = 1;
            else
                p->frame = 0;
        }
        if (p->frame == 0)
            dynamicSprites[0].imageCoordinate.x = 0;
        if (p->frame == 1)
            dynamicSprites[0].imageCoordinate.x = 16;
    }
}
