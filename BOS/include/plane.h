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

#ifndef PLANE_H
#define PLANE_H

#include "utils.h"
#include "image.h"

struct Plane
{
    // Plane
    Image image;

    // Shadow
    Image shadow;
    SDL_Point shadowOffset;

    // Direction
    Direction direction;

    // Speed in pixel/second
    int speed;

    // Margin
    int margin;
};
typedef struct Plane Plane;

struct Mobs{
    Image image;
    int margin;
    Direction direction;
    Image shadow;
    SDL_Point shadowOffset;
    int speed;
};
typedef struct Mobs Mobs;
struct fire{
    Image image;
    int speed;
    int x;
    int y;
};


bool Plane_load(SDL_Renderer *renderer, Plane *plane, const char *image, const char *shadow);
void Plane_destroy(Plane *plane);

void Plane_setX(Plane *plane, int x);
void Plane_setY(Plane *plane, int y);
void Plane_setCoordinates(Plane *plane, int x, int y);

void Plane_moveX(Plane *plane, int x);
void Plane_moveY(Plane *plane, int y);


void Plane_move(Plane *plane, int screenWidth, int screenHeight, int framerate);

void Plane_setDirection(Plane *plane, SDL_Keycode keycode);

void Plane_unsetDirection(Plane *plane, SDL_Keycode keycode);

void Plane_render(SDL_Renderer *renderer, Plane *plane);

//mobs
bool Mobs_load(SDL_Renderer *renderer, Mobs *mob, const char *image);


void Mob_destroy(Mobs *mob);





void Mob_move(Mobs *mob, int framerate);

void Mobs_setDirection(Mobs *mob,int screenWidth,int i);


void Mob_setX(Mobs *mob, int x);

void Mob_setY(Mobs *mob, int y);

void Mob_setCoordinates(Mobs *mob, int x, int y);

void Mob_render(SDL_Renderer *renderer, Mobs *mob);

#endif // PLANE_H
