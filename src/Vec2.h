#ifndef VEC2_H
#define VEC2_H

typedef struct Vec2f {
    float x;
    float y;
} Vec2f;

Vec2f vec2_add(Vec2f a, Vec2f b);
Vec2f vec2_adds(Vec2f a, float b);
Vec2f vec2_sub(Vec2f a, Vec2f b);
Vec2f vec2_subs(Vec2f a, float b);
float vec2_dot(Vec2f a, Vec2f b);
Vec2f vec2_scale(Vec2f a, float b);
Vec2f vec2_normalize(Vec2f a);
float vec2_magnitude(Vec2f a);
float vec2_dist(Vec2f a, Vec2f b);

#endif