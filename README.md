# Helium Engine

## Installation

### Windows Installation

- Install Python from: <https://www.python.org/downloads/>
- Install the latest version of the engine from: <https://github.com/OguzhanUmutlu/helium-game-engine/releases/latest> (The file name should be in the form: Helium-X.X.X-win32.zip)
- Extract the zip you've installed to a folder
- Install MSYS2 from: <https://www.msys2.org/>
- Run `pacman -Syu` in MSYS2 shell
- Run `pacman -S mingw-w64-ucrt-x86_64-gcc`
- Run `pacman -S mingw-w64-x86_64-SDL2`
- Run `pacman -S mingw-w64-x86_64-SDL2_image`
- And that's it! You can now continue on from [here](#making-the-game).

### MacOS/Linux/WSL Installation

- Install the latest version of the engine from: <https://github.com/OguzhanUmutlu/helium-game-engine/releases/latest> (The file name should be in the form: Helium-X.X.X-win32.zip)
- Extract the zip you've installed to a folder
- `sudo apt-get install python3`
- `sudo apt-get install libsdl2-dev`
- `sudo apt-get install libsdl2-image-dev`
- And that's it! You can now continue on from [here](#making-the-game).

## Making the game

First make an empty folder in a place you desire your game to be.

Create a C file(like main.c) and put this in as a starting point:

```c
#include <helium.h>

void Init() {
    printf("Hello, world!");
}

HeliumInit("Title of the game goes here!", Init);
```

Now open a terminal in the engine's folder that you've installed and run this replacing the `path/to/project` with your game's folder path: `python helium.py path/to/project`

Example: `python helium.py C:/Users/User/Desktop/my-game`

Example: `python helium.py ~/my-game`

And that's it! You should now see an executable file in your game folder. (It should be located somewhere like this: my-game/bin/win32/my-game.exe or my-game/bin/linux/my-game)

After running the executable file, if you're on Windows you should see a command prompt pop up with it, you can disable by putting a -w like this: `python helium.py C:/Users/User/Desktop/my-game -w`

### Rendering a sprite

#### Creating a sprite

```c
void Init() {
    Sprite* sprite = SpriteFromPath("./assets/ball.png");
    AddSprite(sprite);
}
```

This will render the `ball.png` image on the top left corner of your screen and it should fall down the screen.

IMPORTANT: Your external files like images should all be located in the `assets` folder!

#### Sprite properties

This is the structure of the sprite:

```c
float x;
float y;
float width;
float height;
Vec2 velocity;
float mass;
bool isStatic;
bool isVisible;
SDL_Texture *texture;
```

Example:

```c
void Init() {
    Sprite* sprite = SpriteFromPath("./assets/ball.png");

    sprite->x = 10;
    sprite->y = 20;
    sprite->width = 50;
    sprite->height = 50;
    // or
    InitSpriteRect(sprite, 10, 20, 50, 50);


    sprite->mass = 10;

    AddSprite(sprite);
}
```

### Making static walls around the screen so that the sprite doesn't fall of the screen

```c
void Init() {
    Sprite* sprite = SpriteFromPath("./assets/ball.png");
    InitSpriteRect(sprite, 10, 20, 50, 50);

    AddSprite(sprite);

    MakeWallsAroundScreen();
}
```

If you don't want to use the shortcut function `MakeWallsAroundScreen()` here's what it does:

```c
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
```

### Implementing game logic

You can create an Update function that runs constantly, or you can create a FixedUpdate function which also runs constantly but in a cycle.

Example usage of both:

```c
Sprite *sprite;

void Update(float deltaTime) { // deltaTime = how long it has been since the last update in seconds
    sprite->x = randf() * SCREEN_WIDTH; // random x value on screen
    sprite->y = randf() * SCREEN_HEIGHT; // random y value on screen
}

void FixedUpdate() {
    sprite->x = randf() * SCREEN_WIDTH; // random x value on screen
    sprite->y = randf() * SCREEN_HEIGHT; // random y value on screen
}

void Init() {
    sprite = SpriteFromPath("./assets/ball.png");
    sprite->isStatic = true; // No gravity or force
    AddSprite(sprite);

    SetUpdate(Update);
    SetFixedUpdate(Update);
}

```

Example game where a ball is forced to the mouse:

```c
#include <helium.h>

Sprite *ball;

void Update(float dt) {
    // If mouse is down
    if (Engine.mouse.left) {
        // Accelerate to the mouse
        float dx = Engine.mouse.x - ball->x;
        float dy = Engine.mouse.y - ball->y;
        ApplyForce(ball, vec2(dx * 10, dy * 10), dt);
    }
}

void Init() {
    // Disable gravity
    Engine.gravity.y = 0;

    // Create the sprite, set its position, dimensions and mass
    ball = SpriteFromPath("./assets/ball.png");
    InitSpriteRect(ball, SCREEN_WIDTH / 2 - 25, 10, 50, 50);
    ball->mass = 1;
    AddSprite(ball);

    MakeWallsAroundScreen();

    // Set the update function
    SetUpdate(Update);
}

// Initialize the game engine
HeliumInit("Ball", Init);
```

## Everything you can use

```c
typedef struct {
    float x;
    float y;
} Vec2;

Vec2 vec2(float x, float y); // Macro
Vec2 Vec2Scale(Vec2 vector, float scalar); // Macro
float Vec2Dot(Vec2 vector1, Vec2 vector2); // Macro
float randf(); // Macro
int DEVICE; // WINDOWS = 0, MACOS = 1, LINUX = 2
double __TimeDouble(); // gives actual time in unix timestamp (in seconds)
float Time(); // gives the time that has passed since the game has started (in seconds)
char *GetExecutablePath(); // gives the executable's path, Engine.gamePath for the directory's path
char *ResolvePath(char *); // resolves path. Example: ./a/b/../../c/d.txt -> c/d.txt  or  /mnt/c/../f.txt -> /mnt/f.txt

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

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
    bool keyboard[SDL_NUM_SCANCODES]; // Example: Engine.keyboard[KEY_A]
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

EngineStruct Engine;

void SetUpdate(F);
void SetFixedUpdate(F);
void SetRender(F);
void SetKeyDown(F);
void SetKeyUp(F);
void SetMouseDown(F);
void SetMouseUp(F);
void SetMouseMove(F);
void SetMouseWheel(F);
void SetQuit(F);

void Crash(...); // Macro. Arguments are same as printf(). Example: Crash("Hello, world: %d\n", 10);

void QuitEngine(int code); // Use this instead of exit()

void WindowSetTitle(char *title); // Macro
void WindowSetIcon(char *path); // Macro

void AddSprite(Sprite *sprite);
void RemoveSprite(Sprite *sprite);
void ClearSprites();

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

void InitSpriteRect(Sprite *sprite, float x, float y, float width, float height); // Macro
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

```
