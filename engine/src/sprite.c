#include "sprite.h"
#include "helium.h"
#include <string.h>

SDL_Texture *LoadTexture(char *path) {
    char *temp = (char *)malloc(strlen(Engine.gamePath) + 1 + strlen(path) + 1);
    if (temp == NULL) {
        Crash(
            "Memory allocation failed for temporary string in LoadTexture.\n");
    }
    temp[0] = '\0';
    strcat(temp, Engine.gamePath);
    strcat(temp, "/");
    strcat(temp, path);
    SDL_Surface *loadedSurface = IMG_Load(temp); // todo: Use ResolvePath?
    if (loadedSurface == NULL) {
        Crash("Unable to load image %s! SDL_image Error: %s\n", path,
              IMG_GetError());
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(Engine.sdl_renderer, loadedSurface);
    if (texture == NULL) {
        Crash("Unable to create texture from %s! SDL Error: %s\n", path,
              SDL_GetError());
    }

    SDL_FreeSurface(loadedSurface);

    free(temp);

    return texture;
}

Sprite *MakeSprite() {
    Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));
    if (sprite == NULL) {
        Crash("Memory allocation failed for sprite.\n");
    }
    sprite->x = 0;
    sprite->y = 0;
    sprite->width = 0;
    sprite->height = 0;
    sprite->velocity = (Vec2){0, 0};
    sprite->mass = 1;
    sprite->isStatic = false;
    sprite->isVisible = false;
    sprite->texture = NULL;
    return sprite;
}

Sprite *SpriteFromTexture(SDL_Texture *texture) {
    Sprite *sprite = MakeSprite();
    int texW, texH;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    sprite->width = texW;
    sprite->height = texH;
    sprite->isVisible = true;
    sprite->texture = texture;
    return sprite;
}

Sprite *SpriteFromPath(char *path) {
    return SpriteFromTexture(LoadTexture(path));
}

Sprite *AddInvisibleWall(float x, float y, float width, float height) {
    Sprite *wall = MakeSprite();
    InitSpriteRect(wall, x, y, width, height);
    wall->isStatic = true;
    AddSprite(wall);
    return wall;
}

void MakeWallsAroundScreen() {
    AddInvisibleWall(0, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
    AddInvisibleWall(-50, 0, 50, SCREEN_HEIGHT);
    AddInvisibleWall(0, -50, SCREEN_WIDTH, 50);
    AddInvisibleWall(SCREEN_WIDTH, 0, 50, SCREEN_HEIGHT);
}

void FreeSprite(Sprite *sprite, bool freeTexture) {
    if (sprite == NULL) {
        return;
    }
    RemoveSprite(sprite);
    if (freeTexture && sprite->texture != NULL) {
        SDL_DestroyTexture(sprite->texture);
    }
    free(sprite);
}

const float FrictionX = 0.005;
const float FrictionY = 0.005;

void SpriteMove(Sprite *sprite, float dx, float dy, float dt) {
    Sprite *col;

    sprite->x += dx;
    col = GetCollidingSprite(sprite);
    if (col != NULL) {
        sprite->x -= dx;
        sprite->velocity.x *= -1;
    }

    sprite->y += dy;
    col = GetCollidingSprite(sprite);
    if (col != NULL) {
        sprite->y -= dy;
        sprite->velocity.y *= -1;
    }
}

void UpdatePhysicsSprite(Sprite *sprite, float dt) {
    if (sprite->isStatic) {
        return;
    }
    ApplyGravity(sprite, dt);
    sprite->velocity.x *= 1 - FrictionX;
    sprite->velocity.y *= 1 - FrictionY;
    float dx = sprite->velocity.x * dt;
    float dy = sprite->velocity.y * dt;
    SpriteMove(sprite, dx, dy, dt);

    if (fabs(sprite->velocity.x) < 0.5) {
        sprite->velocity.x = 0;
    }
    if (fabs(sprite->velocity.y) < 0.5) {
        sprite->velocity.y = 0;
    }
}

void ApplyForce(Sprite *sprite, Vec2 force, float t) {
    float tm = t / sprite->mass;
    sprite->velocity.x += force.x * tm;
    sprite->velocity.y += force.y * tm;
}

void ApplyGravity(Sprite *sprite, float dt) {
    ApplyForce(sprite, Vec2Scale(Engine.gravity, sprite->mass), dt);
}

bool SpritesCollide(Sprite *s1, Sprite *s2) {
    return s1->x < s2->x + s2->width && s1->x + s1->width > s2->x &&
           s1->y < s2->y + s2->height && s1->y + s1->height > s2->y;
}

Sprite *GetCollidingSprite(Sprite *sprite) {
    for (int i = 0; i < Engine.spritesSize; i++) {
        Sprite *target = Engine.sprites[i];
        if (target != sprite && SpritesCollide(sprite, target)) {
            return target;
        }
    }
    return NULL;
}