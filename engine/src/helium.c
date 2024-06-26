#include "helium.h"

#include <stdbool.h>
#include <stdio.h>

EngineStruct Engine;

void EngineUpdate() {
    // todo: I think the program crashes in here when on windows. (i put a
    // printf here and it ran once)
    float now = Time();
    float dt = now - Engine.lastUpdate;
    Engine.lastUpdate = now;
    Engine.fixedUpdateTimer += dt;

    if (Engine.fixedUpdateTimer >= Engine.fixedDeltaTime) {
        Engine.fixedUpdateTimer -= Engine.fixedDeltaTime;
        if (Engine.fixedUpdateTimer >= Engine.fixedDeltaTime) {
            Engine.fixedUpdateTimer = Engine.fixedDeltaTime;
        }
        if (Engine.fixedUpdateFunc != NULL) {
            Engine.fixedUpdateFunc();
        }
        for (int i = 0; i < Engine.spritesSize; i++) {
            Sprite *sprite = Engine.sprites[i];
            UpdatePhysicsSprite(sprite, Engine.fixedDeltaTime);
        }
    }

    if (Engine.updateFunc != NULL) {
        Engine.updateFunc(dt);
    }
}

void EngineRender() {
    SDL_SetRenderDrawColor(Engine.sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(Engine.sdl_renderer);

    if (Engine.renderFunc != NULL) {
        Engine.renderFunc();
    }
    for (int i = 0; i < Engine.spritesSize; i++) {
        Sprite *sprite = Engine.sprites[i];
        if (sprite->texture == NULL || !sprite->isVisible) {
            continue;
        }
        _RenderSprite(sprite);
    }

    SDL_RenderPresent(Engine.sdl_renderer);
}

void QuitEngine(int code) {
    exit(code);
    if (Engine.quitFunc != NULL) {
        Engine.quitFunc(code);
    }
    if (Engine.sdl_renderer != NULL) {
        SDL_DestroyRenderer(Engine.sdl_renderer);
    }
    if (Engine.sdl_window != NULL) {
        SDL_DestroyWindow(Engine.sdl_window);
    }
    for (int i = 0; i < Engine.spritesSize; i++) {
        FreeSprite(Engine.sprites[i], true);
    }
    free(Engine.sprites);
    IMG_Quit();
    SDL_Quit();
    exit(code);
}

void __InitEngine(char *title) {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        Engine.keyboard[i] = false;
    }

    srand(time(NULL));
    Engine.mouse.x = 0;
    Engine.mouse.y = 0;
    Engine.mouse.left = false;
    Engine.mouse.middle = false;
    Engine.mouse.right = false;
    Engine.mouse.wheel.x = 0;
    Engine.mouse.wheel.y = 0;
    Engine.startTime = __TimeDouble();
    Engine.gravity.x = 0;
    Engine.gravity.y = 600;
    Engine.spritesSize = 0;
    Engine.fixedDeltaTime = 0.01;
    Engine.maxFPS = 144;
    Engine.lastUpdate = Time() - 0.001;
    Engine.isRunning = true;
    Engine.fixedUpdateTimer = 0;
    Engine.sdl_renderer = NULL;
    Engine.sdl_window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    Engine.sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                         SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (Engine.sdl_window == NULL) {
        Crash("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    Engine.sdl_renderer =
        SDL_CreateRenderer(Engine.sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (Engine.sdl_renderer == NULL) {
        Crash("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        Crash("SDL_image could not initialize! SDL_image Error: %s\n",
              IMG_GetError());
    }

    Engine.gamePath = ResolvePath(
        strcat(GetExecutablePath(), "/../")); // todo: Use ResolvePath?
    if (Engine.initFunc != NULL) {
        Engine.initFunc();
    }

    SDL_Event event;
    while (Engine.isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                Engine.isRunning = false;
                break;
            case SDL_KEYDOWN:
                Engine.keyboard[event.key.keysym.scancode] = true;
                if (Engine.keyDownFunc != NULL) {
                    Engine.keyDownFunc(event.key.keysym.scancode);
                }
                break;
            case SDL_KEYUP:
                Engine.keyboard[event.key.keysym.scancode] = false;
                if (Engine.keyUpFunc != NULL) {
                    Engine.keyUpFunc(event.key.keysym.scancode);
                }
                break;
            case SDL_MOUSEWHEEL:
                Engine.mouse.wheel.x = event.wheel.x;
                Engine.mouse.wheel.y = event.wheel.y;
                if (Engine.mouseWheelFunc != NULL) {
                    Engine.mouseWheelFunc(event.wheel.x, event.wheel.y);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                case 1:
                    Engine.mouse.left = true;
                    break;
                case 2:
                    Engine.mouse.middle = true;
                    break;
                case 3:
                    Engine.mouse.right = true;
                    break;
                }
                if (Engine.mouseDownFunc != NULL) {
                    Engine.mouseDownFunc(event.button.button - 1);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button) {
                case 1:
                    Engine.mouse.left = false;
                    break;
                case 2:
                    Engine.mouse.middle = false;
                    break;
                case 3:
                    Engine.mouse.right = false;
                    break;
                }
                if (Engine.mouseUpFunc != NULL) {
                    Engine.mouseUpFunc(event.button.button - 1);
                }
                break;
            case SDL_MOUSEMOTION:
                Engine.mouse.x = event.motion.x;
                Engine.mouse.y = event.motion.y;
                if (Engine.mouseMoveFunc != NULL) {
                    Engine.mouseMoveFunc(event.motion.x, event.motion.y);
                }
                break;
            }
        }
        EngineUpdate();
        EngineRender();
        // SDL_Delay(1000 / Engine.maxFPS);
    }

    QuitEngine(0);
}