#ifndef RENDERER_H
#define RENDERER_H

#include "./entity.h"

Player *player_init(Vector2 size, Color color);
void renderer_init(const int width, const int height, const char *title);
void renderer_render(const int width, const int height, Player *player);

#endif // Renderer_h
