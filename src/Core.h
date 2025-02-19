#ifndef CORE_H
#define CORE_H

#include "Vec2.h"
#define DRAG 0.95

typedef struct Body {
    Vec2f position;
    Vec2f velocity;
    Vec2f acceleration;
    float mass;
    float radius;
} Body;

typedef struct Vec2i {
    int x;
    int y;
} Vec2i;

typedef struct BoudingBox {
    Vec2f min;
    Vec2f max;
} BoundingBox;

typedef struct Simulation {
    Body *bodies;
    int num_bodies;
    float dt;
    BoundingBox bounds;
} Simulation;


void update_simulation(Simulation state);
void update_position(Body *a, float dt);
void collide_wall(Body *a, BoundingBox *bounds);
void collide_bodies(Body *a, Body *b);
BoundingBox get_centered_bounds(int width, int height, int screen_width, int screen_height);
Vec2f get_random_position(BoundingBox bounds);

#endif