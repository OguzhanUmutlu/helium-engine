#ifndef DECL_H
#define DECL_H

#include "./sdl_scancode.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

typedef enum { MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT } MouseButton;

typedef struct {
    float x;
    float y;
} Vec2;

#define vec2(x, y)                                                             \
    (Vec2) {                                                                   \
        x, y                                                                   \
    }

#define Vec2Scale(vector, scalar)                                              \
    (Vec2) {                                                                   \
        (scalar * vector.x), (scalar * vector.y)                               \
    }
#define Vec2Dot(v1, v2) (v1.x * v2.x + v1.y * v2.y)

#define randf() ((float)rand() / (float)RAND_MAX)

#define WINDOWS 0
#define MACOS 1
#define LINUX 2

#if defined(_WIN32) || defined(WIN32)
    #define IS_WINDOWS
    #define FASTCALL __fastcall
    #define DEVICE WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define IS_MACOS
    #define FASTCALL __attribute__((fastcall))
    #define DEVICE MACOS
#else
    #define IS_LINUX
    #define FASTCALL
    #define DEVICE LINUX
#endif

double __TimeDouble();
float Time();
char *GetExecutablePath();
char *ResolvePath(char *);

#ifdef IS_WINDOWS
    #include <windows.h>
#else
    #include <limits.h>
    #include <sys/time.h>
    #include <unistd.h>
#endif
#endif