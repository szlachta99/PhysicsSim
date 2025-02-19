#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define scale 3.0
#define strength_scale 10.0
#include "Core.h"

void draw_bounds(SDL_Renderer *renderer, BoundingBox bounds){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, bounds.min.x, bounds.min.y, bounds.max.x, bounds.min.y);
    SDL_RenderLine(renderer, bounds.min.x, bounds.min.y, bounds.min.x, bounds.max.y);
    SDL_RenderLine(renderer, bounds.max.x, bounds.min.y, bounds.max.x, bounds.max.y);
    SDL_RenderLine(renderer, bounds.min.x, bounds.max.y, bounds.max.x, bounds.max.y);
}

typedef struct appState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int running;
    int width;
    int height;
    int last_time;
    float angle;
    float strength;

    Simulation sim;
} AppState;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    AppState *state = malloc(sizeof(AppState));
    state->width = 800;
    state->height = 600;
    state->running = 1;
    state->last_time = SDL_GetTicks();
    
    state->sim.dt = 0;
    state->sim.bounds = get_centered_bounds(213 * scale, 122 * scale, state->width, state->height);
    state->sim.num_bodies = 16;
    state->sim.bodies = malloc(sizeof(Body) * state->sim.num_bodies);

    Vec2f pos = {state->sim.bounds.min.x + state->sim.bounds.max.x / 2, state->sim.bounds.min.y + state->sim.bounds.max.y / 2};
    float radius = 5.2 * scale / 2;
    state->sim.bodies[0].position = pos;
    state->sim.bodies[0].position.x -= 3 * radius;
    state->sim.bodies[0].velocity = (Vec2f){0, 0};
    state->sim.bodies[0].mass = 1;
    state->sim.bodies[0].radius = radius;
    pos.x += 3 * radius;
    int c = 1;
    for( int i = 0; i < 5; i++){
        for( int j = 0; j < i; j++){
            state->sim.bodies[c].position = (Vec2f){pos.x + i * 2 * radius, pos.y + j * 2 * radius - i * radius};
            state->sim.bodies[c].velocity = (Vec2f){0, 0};
            state->sim.bodies[c].mass = 1;
            state->sim.bodies[c++].radius = radius;
        }
    }


    SDL_CreateWindowAndRenderer("Simulation", state->width, state->height, SDL_WINDOW_RESIZABLE, &state->window, &state->renderer);
    
    state->texture = SDL_CreateTexture(state->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, state->width, state->height);

    if(!state->window || !state->renderer || !state->texture) {
        SDL_Log("Failed to create window, renderer, or texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = appstate;

    int current_time = SDL_GetTicks();
    state->sim.dt = (current_time - state->last_time) / 1000.0f;
    state->sim.dt = state->sim.dt > 0.1f ? 0.1f : state->sim.dt;
    state->last_time = current_time;
    update_simulation(state->sim);

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    draw_bounds(state->renderer, state->sim.bounds);
    bool draw_stick = 1;

    for (int i = 0; i < state->sim.num_bodies; i++) {
        Body *body = &state->sim.bodies[i];
        Vec2i screen_pos = (Vec2i){(int)body->position.x, (int)body->position.y};
        int radius = body->radius;
        
        if(body->velocity.x != 0 || body->velocity.y != 0){
            draw_stick = 0;
        }

        SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        if(i == 0)
            SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // horizontal offset
                int dy = radius - h; // vertical offset
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderPoint(state->renderer, screen_pos.x + dx, screen_pos.y + dy);
                }
            }
        }
    }

    if(draw_stick){
        SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
        SDL_RenderLine(state->renderer, state->sim.bodies[0].position.x, state->sim.bodies[0].position.y, state->sim.bodies[0].position.x - cos(state->angle) * state->strength / strength_scale, state->sim.bodies[0].position.y - sin(state->angle) * state->strength / strength_scale);
    }

    SDL_SetRenderDrawColor(state->renderer, 255, 0, 0, 255);
    char buffer[100];
    sprintf(buffer, "FPS: %.2f", 1.0f / state->sim.dt);
    SDL_RenderDebugText(state->renderer, 0, 0, buffer);

    SDL_RenderPresent(state->renderer);
    return state->running ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = appstate;
    if (event->type == SDL_EVENT_QUIT) {
        state->running = 0;
    }
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        state->width = event->window.data1;
        state->height = event->window.data2;
        state->sim.bounds = get_centered_bounds(213 * scale, 122 * scale, state->width, state->height);
    }
    if (event->type == SDL_EVENT_MOUSE_MOTION) {
        Vec2f mouse = (Vec2f){event->motion.x, event->motion.y};
        state->angle = atan2(mouse.y - state->sim.bodies[0].position.y, mouse.x - state->sim.bodies[0].position.x);
        state->strength = vec2_dist(mouse, state->sim.bodies[0].position) * -strength_scale;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            state->sim.bodies[0].velocity = vec2_scale((Vec2f){cos(state->angle), sin(state->angle)}, state->strength);
        }
    }
    
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (result == SDL_APP_FAILURE) {
        SDL_Log("An error occurred, quitting");
    }
    else{
        AppState *state = appstate;
        SDL_DestroyTexture(state->texture);
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        free(state->sim.bodies);
        free(state);
    }
}