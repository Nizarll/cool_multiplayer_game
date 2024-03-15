#ifndef EASE_H
#define EASE_H

#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef enum {
  eIOELASTIC,
  eIOSINE,
  eIOQUART,
  eIOBACK,
  eIOCIRCLE,
  // ...
} Kind;

typedef struct Ease Ease;
Ease ease(Kind kind, Vector2 from, Vector2 to);
float ease_inout_back(float a);
float ease_inout_circle(float a);
float ease_inout_sine(float a);
float ease_inout_circ(float a);

#endif // EASE_H
