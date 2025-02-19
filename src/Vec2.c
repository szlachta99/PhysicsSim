#include "Vec2.h"
#include <math.h>

Vec2f vec2_add(Vec2f a, Vec2f b)
{
    return (Vec2f){a.x + b.x, a.y + b.y};
}

Vec2f vec2_sub(Vec2f a, Vec2f b)
{
    return (Vec2f){a.x - b.x, a.y - b.y};
}

Vec2f vec2_subs(Vec2f a, float b)
{
    return (Vec2f){a.x - b, a.y - b};
}

Vec2f vec2_adds(Vec2f a, float b)
{
    return (Vec2f){a.x + b, a.y + b};
}

float vec2_dot(Vec2f a, Vec2f b)
{
    return a.x * b.x + a.y * b.y;
}

Vec2f vec2_scale(Vec2f a, float b)
{
    return (Vec2f){a.x * b, a.y * b};
}

Vec2f vec2_normalize(Vec2f a)
{
    float mag = vec2_magnitude(a);
    return (Vec2f){a.x / mag, a.y / mag};
}

float vec2_magnitude(Vec2f a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

float vec2_dist(Vec2f a, Vec2f b)
{
    return sqrt(
          (a.x - b.x) * (a.x - b.x) 
        + (a.y - b.y) * (a.y - b.y));
}
