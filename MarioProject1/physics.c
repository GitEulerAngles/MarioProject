#include "physics.h"

struct aabb createBounds(struct Vector2f pos, struct Vector2i dim, struct Vector2f offSet) {
    struct aabb r = { pos.x + offSet.x, pos.y + offSet.y, pos.x + dim.x - offSet.x, pos.y + dim.y };
    return r;
}

_Bool checkCollision(struct aabb b1, struct aabb b2) {
    if (b1.max.x < b2.min.x || b1.min.x > b2.max.x) return 0;
    if (b1.max.y < b2.min.y || b1.min.y > b2.max.y) return 0;

    return 1;
}

struct Vector2i getSide(struct aabb b1, struct aabb b2) {
    struct Vector2i v = { 0,0 };
    struct Vector2f center = { (b1.min.x + b1.max.x) / 2, (b1.min.y + b1.max.y) / 2 };
    if (b1.max.x > b2.min.x && b1.min.x < b2.max.x) {
        if (b2.max.y < center.y)
            v.y += 1;
        else if (b2.min.y > center.y)
            v.y = -1;
    }
    else if (b1.max.y > b2.min.y && b1.min.y < b2.max.y) {
        if (b2.max.x < center.x)
            v.x = 1;
        else if (b2.min.x > center.x)
            v.x = -1;
    }
    return v;
}

void applyGravity(_Bool grounded, struct Vector2f *vel) {
    if (grounded)
        vel->y = 1;
    else {
        vel->y += 1;

        if (vel->y > 48)
            vel->y = 48;
    }
}

struct Vector2f calculateNormal(struct aabb b1, struct aabb b2) {
    struct Vector2f center1 = { (b1.min.x + b1.max.x) / 2, (b1.min.y + b1.max.y) / 2 };
    struct Vector2f center2 = { (b2.min.x + b2.max.x) / 2, (b2.min.y + b2.max.y) / 2 };
    struct Vector2f result = { center2.x - center1.x, center2.y - center1.y  };
    return result;
}

struct Vector2f calculateOverlap(struct aabb b1, struct aabb b2, struct Vector2f normal) {
    struct Vector2f overlap = { 0,0 };

    float a_extent = (b1.max.x - b1.min.x) / 2;
    float b_extent = (b2.max.x - b2.min.x) / 2;

    overlap.x = a_extent + b_extent - abs(normal.x);

    if (overlap.x < 0) {
        overlap.x = 0;
        return overlap;
    }

    a_extent = (b1.max.y - b1.min.y) / 2;
    b_extent = (b2.max.y - b2.min.y) / 2;

    overlap.y = a_extent + b_extent - abs(normal.y);

    if (overlap.y < 0)
        overlap.y = 0;

    return overlap;
}

struct Vector2f resolveCollision(struct aabb b1, struct aabb b2, struct aabb precision, _Bool* grounded) {
    struct Vector2f r = { 0,0 };

    if (!checkCollision(b1, b2))
        return r;

    struct Vector2f n = calculateNormal(b1,b2);
    r = calculateOverlap(b1, b2, n);

    if (precision.max.x == -999) {
        if (r.x < r.y) {
            if (n.x < 0)
                r.x *= -1;
            r.y = 0;
        }
        else {
            if (n.y < 0)
                r.y *= -1;
            r.x = 0;
        }
    }
    else {
        struct Vector2i side = getSide(b2, precision);

        r.x *= side.x;
        r.y *= side.y;
    }

    return r;
}
