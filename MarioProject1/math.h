#ifndef MATH
#define MATH

#include <math.h>

struct Vector2i {
    int x;
    int y;
};

int vectorToInt(int x, int y);
struct Vector2i intToVector(int x);
struct Vector2i addVectors(struct Vector2i v1, struct Vector2i v2);
char vectorToString(struct Vector2i v);

#endif