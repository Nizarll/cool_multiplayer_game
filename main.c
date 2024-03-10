#include <stdint.h>
#include <stdio.h>

#include "libs/renderer.h"
#include <ctype.h>
#include <raylib.h>
#include <raymath.h>
#include <signal.h>

static const int screenWidth = 800;
static const int screenHeight = 450;

int main(void) {
  renderer_init(screenWidth, screenHeight, "cool pool window");
  return 0;
}
