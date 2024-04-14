#ifndef MATH
#define MATH

struct Vector2i {
    int x;
    int y;
};

struct Vector2i addVectors(struct Vector2i v1, struct Vector2i v2);
char vectorToString(struct Vector2i v);

#endif