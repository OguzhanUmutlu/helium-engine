#ifndef MAIN_H
#define MAIN_H

#include "decl.h"
#include "math.h"
#include "objectmanager.h"
#include "sdl_scancode.h"
#include "sprite.h"
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef void (*InitFunction_t)(void);
typedef void (*UpdateFunction_t)(float deltaTime);
typedef void (*FixedUpdateFunction_t)(void);
typedef void (*RenderFunction_t)(void);
typedef void (*KeyDownFunction_t)(KeyboardButton key);
typedef void (*KeyUpFunction_t)(KeyboardButton key);
typedef void (*MouseDownFunction_t)(MouseButton button);
typedef void (*MouseUpFunction_t)(MouseButton button);
typedef void (*MouseMoveFunction_t)(int x, int y);
typedef void (*MouseWheelFunction_t)(int x, int y);
typedef void (*QuitFunction_t)(int code);

typedef struct {
    int argc;
    char **argv;
    char *gamePath;
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    bool keyboard[SDL_NUM_SCANCODES];
    struct {
        int x;
        int y;
        bool left;
        bool middle;
        bool right;
        Vec2 wheel;
    } mouse;
    Sprite **sprites;
    size_t spritesSize;
    Vec2 gravity;
    float fixedDeltaTime;
    int maxFPS;
    float lastUpdate;
    bool isRunning;
    float fixedUpdateTimer;
    double startTime;
    // TODO: Try using `extern void Init();` and then see if it can handle it
    InitFunction_t initFunc;
    UpdateFunction_t updateFunc;
    FixedUpdateFunction_t fixedUpdateFunc;
    RenderFunction_t renderFunc;
    KeyDownFunction_t keyDownFunc;
    KeyUpFunction_t keyUpFunc;
    MouseDownFunction_t mouseDownFunc;
    MouseUpFunction_t mouseUpFunc;
    MouseMoveFunction_t mouseMoveFunc;
    MouseWheelFunction_t mouseWheelFunc;
    QuitFunction_t quitFunc;
} EngineStruct;

extern EngineStruct Engine;

#define SetUpdate(F) Engine.updateFunc = F
#define SetFixedUpdate(F) Engine.fixedUpdateFunc = F
#define SetRender(F) Engine.renderFunc = F
#define SetKeyDown(F) Engine.keyDownFunc = F
#define SetKeyUp(F) Engine.keyUpFunc = F
#define SetMouseDown(F) Engine.mouseDownFunc = F
#define SetMouseUp(F) Engine.mouseUpFunc = F
#define SetMouseMove(F) Engine.mouseMoveFunc = F
#define SetMouseWheel(F) Engine.mouseWheelFunc = F
#define SetQuit(F) Engine.quitFunc = F

#define WindowSetTitle(title) SDL_SetWindowTitle(Engine.sdl_window, title)
#define WindowSetIcon(path)                                                    \
    do {                                                                       \
        SDL_Surface *s = IMG_Load(temp);                                       \
        SDL_SetWindowIcon(Engine.sdl_window, s);                               \
        SDL_FreeSurface(s);                                                    \
    } while (0)

#define Crash(...)                                                             \
    printf("Engine Crashed\n" __VA_ARGS__);                                    \
    QuitEngine(1)

void __InitEngine(char *title);
void QuitEngine(int code);

#define HeliumInit(title, Init)                                                \
    int main(int argc, char **argv) {                                          \
        Engine.initFunc = Init;                                                \
        Engine.argc = argc;                                                    \
        Engine.argv = argv;                                                    \
        __InitEngine(title);                                                   \
    }

#endif