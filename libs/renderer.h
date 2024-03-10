#ifndef RENDERER_H
#define RENDERER_H

#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Object Object;
typedef struct AnimationHandler AnimationHandler;

void renderer_init(const int width, const int height, const char *title);

#endif // Renderer_h
