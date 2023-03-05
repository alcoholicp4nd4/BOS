/*
 * Copyright (c) 2018 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "animation.h"

bool intersect_rects(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w < b.x || a.x > b.x + b.w || a.y + a.h < b.y || a.y > b.y + b.h) {
        return false;
    }
    return true;
}
bool Animation_start(SDL_Renderer *renderer, int width, int height)
{
    Animation animation = {0};
    animation.renderer  = renderer;
    animation.width     = width;
    animation.height    = height;

    // Loat animation assets
    if(!Animation_load(&animation))
        return false;


    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 100);

    // Initialize start frame
    int frame = SDL_getFramecount(&fpsmanager);


    // Event loop exit flags
    bool quit = false;
    bool is_dead0=intersect_rects(animation.plane.image.rect,animation.mob[0].image.rect);
    bool is_dead1=intersect_rects(animation.plane.image.rect,animation.mob[1].image.rect);
    bool is_dead2=intersect_rects(animation.plane.image.rect,animation.mob[2].image.rect);
    bool is_dead3=intersect_rects(animation.plane.image.rect,animation.mob[3].image.rect);
    bool is_dead4=intersect_rects(animation.plane.image.rect,animation.mob[4].image.rect);

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e)||is_dead0||is_dead1||is_dead2||is_dead3||is_dead4)
        {
            // Handle animation events
            quit = Animation_handleEvent(&animation, &e);
        }

        // Each new frame
        int currentFrame = SDL_getFramecount(&fpsmanager);
        if(currentFrame != frame)
        {
            // Update animation frame
            Animation_update(&animation, SDL_getFramerate(&fpsmanager));

            currentFrame = frame;
        }


        // Render animation
        Animation_render(&animation);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }


    Animation_destroy(&animation);


    return true;
}



bool Animation_load(Animation *animation)
{
    // Load the ocean image
    if(!Background_load(animation->renderer, &animation->background, OCEAN_IMG))
        return false;

    // Background move direction
    animation->background.direction = DIRECTION_DOWN;

    // Background speed in pixel/second
    animation->background.speed =OCEAN_SPEED;

    if(!Plane_load(animation->renderer, &animation->plane, PLANE_IMG, PLANE_SHADOW))
    {
        Background_destroy(&animation->background);
        return false;
    }

    animation->plane.margin = PLANE_MARGIN;
    animation->plane.shadowOffset.x = PLANE_SHADOW_X;
    animation->plane.shadowOffset.y = PLANE_SHADOW_Y;

    // Set plane initial position
    Plane_setX(&animation->plane, (animation->width - animation->plane.image.rect.w) / 2);
    Plane_setY(&animation->plane, animation->height - animation->plane.image.rect.h - animation->plane.margin);
    for (int i=0;i<5;i++){
    if(!Mobs_load(animation->renderer, &animation->mob[i], MOB_IMG))
    {
        Background_destroy(&animation->background);
        return false;
    }


    Mob_setX(&animation->mob[i], (animation->width- animation->mob[i].image.rect.w-(i+1)*100));
    Mob_setY(&animation->mob[i], (animation->height - animation->mob[i].image.rect.h)-(i+1)*100);
    animation->mob[i].speed= MOB_SPEED;
    animation->mob[i].direction= DIRECTION_STOP;
}

    // Plane move direction
    animation->plane.direction = DIRECTION_STOP;

    // Plane move speed in pixel/second
    animation->plane.speed = PLANE_SPEED;

    return true;
}

void Animation_destroy(Animation *animation)
{
    Plane_destroy(&animation->plane);
    Background_destroy(&animation->background);
}

bool Animation_handleEvent(Animation *animation, SDL_Event *e)
{
    bool quit = false;


    // User requests quit
    if(e->type == SDL_QUIT)
    {
        quit = true;
    }
    else if(e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_ESCAPE:
            quit = true;
            break;

        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
            Plane_setDirection(&animation->plane, e->key.keysym.sym);
            break;
        }
    }
    else if(e->type == SDL_KEYUP)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_RIGHT:
        case SDLK_LEFT:
            Plane_unsetDirection(&animation->plane, e->key.keysym.sym);
            break;
        }
    }

    return quit;
}
void Animation_update(Animation *animation, int framerate)
{
    // Move ocean
    Background_move(&animation->background, framerate);
    // Move plane
    for (int i=0;i<5;i++){
    Mobs_setDirection(&animation->mob[i],animation->width,i);
    Mob_move(&animation->mob[i],framerate);
    }
    Plane_move(&animation->plane, animation->width, animation->height, framerate);

}

void Animation_render(Animation *animation)
{
    // Render ocean
    Background_render(animation->renderer, &animation->background, animation->width, animation->height);

    // Render plane
    Plane_render(animation->renderer, &animation->plane);
    for (int i=0;i<5;i++){
    Mob_render(animation->renderer, &animation->mob[i]);
    }
}
