#include "window.h"
#include <stdio.h>

struct playerAnimation createAnimation() {
    struct playerAnimation newP;
    newP.type = IDLE;
    newP.frame = 0;
    newP.delay = 16;
    return newP;
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

    int ch, count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch >= '0' && ch <= '9') {
            char str[2] = { ch, '\0' };
            blockTypes[count] = atoi(str);
            count++;

            if (count >= 100) {
                fprintf(stderr, "Error: Too many block types in file.\n");
                break;
            }
        }
    }

    fclose(file);
}

void updateBlocks() {
    for (int i = 0; i < 100; i++) {
        struct Vector2i coords = intToVector(i);

        if (blockTypes[i] == QUESTION) {
            staticSprites[i].imageDimension.x = 16;
            staticSprites[i].imageDimension.y = 16;
            staticSprites[i].pos.x = coords.x * 10 * MULTIPLIER;
            staticSprites[i].pos.y = coords.y * 10 * MULTIPLIER;
            staticSprites[i].dim.x = 10 * MULTIPLIER;
            staticSprites[i].dim.y = 10 * MULTIPLIER;
        }
        else if (blockTypes[i] == GRASS) {
            staticSprites[i].imageDimension.x = 16;
            staticSprites[i].imageDimension.y = 16;

            int neighbors[4][2] = { {0,-1},{1,0},{0,1},{-1,0} };
            int count = 0;

            // Get neighboring squares
            for (int i = 0; i < 4; i++) {
                struct Vector2i new_coord = { coords.x + neighbors[i][0], coords.y + neighbors[i][1] };

                if (new_coord.x < 0 || new_coord.y < 0 || 
                    new_coord.x > 9 || new_coord.y > 9)
                    continue;

                if (blockTypes[vectorToInt(new_coord.x, new_coord.y)] == GRASS)
                    count += pow(2, i);
            }

            staticSprites[i].imageCoordinate.x = (count % 3) * 16;
            staticSprites[i].imageCoordinate.y = (floor(count / 3) + 1) * 16;

            staticSprites[i].pos.x = coords.x * 10 * MULTIPLIER;
            staticSprites[i].pos.y = coords.y * 10 * MULTIPLIER;
            staticSprites[i].dim.x = 10 * MULTIPLIER;
            staticSprites[i].dim.y = 10 * MULTIPLIER;
        }
        else {
            staticSprites[i].imageDimension.x = 0;
            staticSprites[i].imageDimension.y = 0;
            staticSprites[i].pos.x = coords.x * 10 * MULTIPLIER;
            staticSprites[i].pos.y = coords.y * 10 * MULTIPLIER;
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
        if (p->delay > 8) {
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
