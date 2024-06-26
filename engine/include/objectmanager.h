#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "decl.h"
#include "sprite.h"

#define _RenderSprite(sprite)                                                  \
    SDL_RenderCopy(                                                            \
        Engine.sdl_renderer, sprite->texture, NULL,                            \
        &(SDL_Rect){sprite->x, sprite->y, sprite->width, sprite->height})

void _ListAdd(void ***list, size_t *listSize, size_t elementSize,
              void *element);
void _ListRemoveIndex(void **list, size_t *listSize, int index);
void _ListRemove(void **list, size_t *listSize, void *element);
void _ListClear(void **list, size_t *listSize);
void AddSprite(Sprite *sprite);
void RemoveSprite(Sprite *sprite);
void ClearSprites();

#endif