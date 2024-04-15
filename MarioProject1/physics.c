#include "physics.h"

struct aabb createBounds(struct Vector2i pos, struct Vector2i dim) {
    struct aabb r = { pos.x, pos.y, pos.x + dim.x, pos.y + dim.y };
    return r;
}

_Bool checkCollision(struct aabb b1, struct aabb b2) {
    if (b1.max.x < b2.min.x || b1.min.x > b2.max.x) return 0;
    if (b1.max.y < b2.min.y || b1.min.y > b2.max.y) return 0;

    return 1;
}

struct Vector2i resolveCollision(struct aabb b1, struct aabb b2) {

}

struct Vector2i processStaticPhysics(struct aabb dynamic_object, struct aabb static_object) {
    struct Vector2i r = { 0,0 };

    if (!checkCollision(dynamic_object, static_object))
        return r;
    

}
