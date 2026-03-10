#include "raylibFunctions.h"
#include <stdio.h>
int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "fenetre de Tests");

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ZERO)) resetCamera(&camera);

        zoom(&camera);


        
        BeginDrawing();
            BeginMode2D(camera);

            ClearBackground(WHITE);
            DrawRectangle(50,100,10,100,BLACK);
            DrawPixel(150,150,BLACK);

            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}