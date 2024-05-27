#ifndef MATH
#define MATH

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Vector {
    int16_t* data;
    uint16_t size;
    uint16_t capacity;
    uint16_t end;
};

struct Vector2i {
    int16_t x;
    int16_t y;
};

struct Vector2f {
    float x;
    float y;
};

void resizeVector(struct Vector* vec, int16_t capacity);
void freeVector(struct Vector* vec);
void addElement(struct Vector* vec, int16_t element); 
int16_t getElement(struct Vector* vec, int16_t index);
struct Vector initVector(uint16_t capacity);
int vectorToInt(int x, int y, int l);
struct Vector2i intToVector(int x, int l);
struct Vector2i addVectors(struct Vector2i v1, struct Vector2i v2);
char vectorToString(struct Vector2i v);

#endif