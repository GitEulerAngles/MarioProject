#ifndef PHYSICS
#define PHYSICS

#include "math.h"

struct aabb {
    struct Vector2i min;
    struct Vector2i max;
};

struct aabb boxes[1];

struct aabb createBounds(struct Vector2i pos, struct Vector2i dim);
_Bool checkCollision(struct aabb b1, struct aabb b2);


#endif // !PHYSICS
