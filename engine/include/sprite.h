#ifndef SPRITE_H
#define SPRITE_H

#include "decl.h"
#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float width;
    float height;
    Vec2 velocity;
    float mass;
    bool isStatic;
    bool isVisible;
    SDL_Texture *texture;
} Sprite;

#define InitSpriteRect(sprite, x_, y_, width_, height_)                        \
    sprite->x = x_;                                                            \
    sprite->y = y_;                                                            \
    sprite->width = width_;                                                    \
    sprite->height = height_

Sprite *MakeSprite();
SDL_Texture *LoadTexture(char *path);
Sprite *SpriteFromTexture(SDL_Texture *texture);
Sprite *SpriteFromPath(char *path);
Sprite *AddInvisibleWall(float x, float y, float width, float height);
void MakeWallsAroundScreen();
void FreeSprite(Sprite *sprite, bool freeTexture);

void UpdatePhysicsSprite(Sprite *sprite, float dt);
void ApplyForce(Sprite *sprite, Vec2 force, float t);
void ApplyGravity(Sprite *sprite, float dt);
bool SpritesCollide(Sprite *s1, Sprite *s2);
Sprite *GetCollidingSprite(Sprite *sprite);

#endif