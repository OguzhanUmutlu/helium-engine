#include "objectmanager.h"

#include <stdio.h>

#include "helium.h"

void _ListAdd(void ***list, size_t *listSize, size_t elementSize,
              void *element) {
    if (element == NULL) {
        return;
    }
    (*listSize)++;
    *list = realloc(*list, (*listSize) * elementSize);
    if (*list == NULL) {
        Crash("Failed to reallocate memory for sprite/body list\n");
    }
    (*list)[(*listSize) - 1] = element;
}

void _ListRemoveIndex(void **list, size_t *listSize, int index) {
    if (index >= *listSize) {
        return;
    }
    list[index] = NULL;
    for (int i = index; i < *listSize; i++) {
        list[i] = list[i + 1];
    }
    (*listSize)--;
}

void _ListRemove(void **list, size_t *listSize, void *element) {
    for (int i = 0; i < *listSize; i++) {
        if (list[i] == element) {
            _ListRemoveIndex(list, listSize, i);
            break;
        }
    }
}

void _ListClear(void **list, size_t *listSize) {
    for (int i = 0; i < *listSize; i++) {
        list[i] = NULL;
    }
    *listSize = 0;
}

void AddSprite(Sprite *sprite) {
    _ListAdd((void ***)(&Engine.sprites), &Engine.spritesSize, sizeof(Sprite),
             (void *)sprite);
}

void RemoveSprite(Sprite *sprite) {
    _ListRemove((void **)Engine.sprites, &Engine.spritesSize, (void *)sprite);
}

void ClearSprites() {
    _ListClear((void **)Engine.sprites, &Engine.spritesSize);
}
