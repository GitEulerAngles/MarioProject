#ifndef PHYSICS
#define PHYSICS

#include "math.h"

struct aabb {
    struct Vector2i min;
    struct Vector2i max;
};

struct aabb dynamicBoxes[1];
struct aabb staticBoxes[121];

void applyGravity(_Bool grounded, float* vely);
struct aabb createBounds(struct Vector2f pos, struct Vector2i dim);
_Bool checkCollision(struct aabb b1, struct aabb b2);
struct Vector2f resolveCollision(struct aabb b1, struct aabb b2, _Bool dynamicCollision);

#endif // !PHYSICS
