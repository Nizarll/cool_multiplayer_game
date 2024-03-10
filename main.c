#include <stdint.h>
#include <stdio.h>

#include "libs/renderer.h"
#include <raylib.h>
#include <raymath.h>

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  renderer_init(screenWidth, screenHeight, "cool pool window");
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    printf("YOUR FPS IS : %i \n", GetFPS());
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);
    EndDrawing();
  }
  CloseWindow(); // Close window and OpenGL context
  return 0;
}
