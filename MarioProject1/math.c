#include "math.h"

struct Vector2i addVectors(struct Vector2i v1, struct Vector2i v2) {
    return (struct Vector2i) { v1.x + v2.x, v1.y + v2.y };
}
char vectorToString(struct Vector2i v) {
    return ("%d, %d", v.x, v.y);
}