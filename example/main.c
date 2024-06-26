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