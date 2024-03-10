#ifndef RENDERER_H
#define RENDERER_H

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

typedef struct Object Object;
typedef struct AnimationHandler AnimationHandler;

void renderer_init(const int width, const int height, const char *title);

#endif // Renderer_h
