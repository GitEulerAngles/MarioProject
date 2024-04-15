#include "math.h"

int vectorToInt(int x, int y) {
    return (y * 10) + x;
}
struct Vector2i intToVector(int x) {
    struct Vector2i r = { x % 10, floor(x / 10) };
    return r;
}
struct Vector2i addVectors(struct Vector2i v1, struct Vector2i v2) {
    return (struct Vector2i) { v1.x + v2.x, v1.y + v2.y };
}
char vectorToString(struct Vector2i v) {
    return ("%d, %d", v.x, v.y);
}
