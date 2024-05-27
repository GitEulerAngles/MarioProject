#include "math.h"

struct Vector initVector(uint16_t capacity) {
    struct Vector v;
    v.data = malloc(capacity * sizeof(int16_t));
    if (v.data == NULL) {
        printf("Could not allocate memory to new vector.\n");
        return v;
    }

    v.size = 0;
    v.end = 0;
    v.capacity = capacity;
    return v;
}
void freeVector(struct Vector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}
void resizeVector(struct Vector* vec, int16_t capacity) {
    int16_t* data = realloc(vec->data, capacity * sizeof(int16_t));
    if (data == NULL) {
        printf("Could not reallocate memory of a vector.\n");
        return;
    }
    vec->data = data;
    vec->capacity = capacity;
}
void addElement(struct Vector* vec, int16_t element) {
    if (vec->size == vec->capacity) {
        resizeVector(vec, vec->capacity * 2);
    }

    vec->data[vec->size] = element;
    vec->size++;
}
int16_t getElement(struct Vector* vec, int16_t index) {
    return vec->data[index];
}
int vectorToInt(int x, int y, int l) {
    return (y * l) + x;
}
struct Vector2i intToVector(int x, int l) {
    struct Vector2i r = { x % l, floor(x / (float)l) };
    return r;
}
struct Vector2i addVectors(struct Vector2i v1, struct Vector2i v2) {
    return (struct Vector2i) { v1.x + v2.x, v1.y + v2.y };
}
char vectorToString(struct Vector2i v) {
    return ("%d, %d", v.x, v.y);
}
