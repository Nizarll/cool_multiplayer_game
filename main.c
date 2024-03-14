#include "libs/renderer.h"
#include <raylib.h>

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {
  renderer_init(screenWidth, screenHeight, "window");
  Player *player = player_init(
      (Vector2){
          .x = 4,
          .y = 4,
      },
      WHITE);
  renderer_render(screenWidth, screenHeight, player);
  return 0;
}
