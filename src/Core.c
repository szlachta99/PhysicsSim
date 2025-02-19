#include "Core.h"
#include <stdlib.h>
void update_simulation(Simulation state){
    for(int i = 0; i < state.num_bodies; i++){
        Body *a = &state.bodies[i];
        collide_wall(a, &state.bounds);
        a->velocity = vec2_scale(a->velocity, DRAG);
        if(vec2_magnitude(a->velocity) < 0.01){
            a->velocity = (Vec2f){0, 0};
        }
        update_position(a, state.dt);

        for(int j = i + 1; j < state.num_bodies; j++){
            Body *b = &state.bodies[j];
            collide_bodies(a, b);
        }
    }
}

void collide_wall(Body *a, BoundingBox *bounds){
    if(a->position.x - a->radius < bounds->min.x){
        a->position.x = bounds->min.x + a->radius;
        a->velocity.x = -a->velocity.x;
    }
    if(a->position.x + a->radius > bounds->max.x){
        a->position.x = bounds->max.x - a->radius;
        a->velocity.x = -a->velocity.x;
    }
    if(a->position.y - a->radius < bounds->min.y){
        a->position.y = bounds->min.y + a->radius;
        a->velocity.y = -a->velocity.y;
    }
    if(a->position.y + a->radius > bounds->max.y){
        a->position.y = bounds->max.y - a->radius;
        a->velocity.y = -a->velocity.y;
    }
}

void update_position(Body *a, float dt){
    a->position = vec2_add(a->position, vec2_scale(a->velocity, dt));
}


void collide_bodies(Body *a, Body *b){
    float dist = vec2_dist(a->position, b->position);
    if(dist < a->radius + b->radius){
        Vec2f impactVector = vec2_sub(a->position, b->position);
        float overlap = dist - a->radius - b->radius;
        Vec2f dir = impactVector;
        dir = vec2_normalize(dir);
        dir = vec2_scale(dir, overlap / 2);
        b->position = vec2_add(b->position, dir);
        a->position = vec2_sub(a->position, dir);
        
        dist = a->radius + b->radius;
        impactVector = vec2_normalize(impactVector);
        impactVector = vec2_scale(impactVector, dist);

        float totalMass = a->mass + b->mass;
        Vec2f vDiff = vec2_sub(a->velocity, b->velocity);

        float num = vec2_dot(vDiff, impactVector);
        float den = totalMass * dist * dist;
        
        Vec2f deltaV;
        // a
        deltaV = vec2_scale(impactVector, -2 * b->mass * num / den);
        a->velocity = vec2_add(a->velocity, deltaV);
        // b
        deltaV = vec2_scale(impactVector, 2 * a->mass * num / den);
        b->velocity = vec2_add(b->velocity, deltaV);
    }
}

BoundingBox get_centered_bounds(int width, int height, int screen_width, int screen_height){
    BoundingBox bounds;
    bounds.min.x = (screen_width - width) / 2;
    bounds.min.y = (screen_height - height) / 2;
    bounds.max.x = bounds.min.x + width;
    bounds.max.y = bounds.min.y + height;
    return bounds;
}

Vec2f get_random_position(BoundingBox bounds)
{
    Vec2f position;
    position.x = bounds.min.x + SDL_rand((int)(bounds.max.x - bounds.min.x));
    position.y = bounds.min.y + SDL_rand((int)(bounds.max.y - bounds.min.y));
    return position;
}
