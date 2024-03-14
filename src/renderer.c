#include "../libs/renderer.h"
#include <raylib.h>
#include <raymath.h>

#define FPS_CAP (4999)
#define PLAYER_HEIGHT (12)
#define PLAYER_WIDTH (6)

#define OPT(T) (option(T))
// void handle_movement(Entity *entity) {
//   if (IsKeyDown(KEY_A)) {
//     entity->position = Vector2Add(
//         entity->position,
//         (Vector2){.x = -(float)1 / entity->input_tracker->keys_pressed,
//                   .y = 0});
//   }
//   if (IsKeyDown(KEY_D)) {
//     entity->position = Vector2Add(
//         entity->position,
//         (Vector2){.x = (float)1 / entity->input_tracker->keys_pressed, .y =
//         0});
//   }
// }
//
const Animation walk = (Animation){
    .path = "./resources/character_walk_cycle.png",
    .length = 4,
    .speed = 1,
};

static const Animation idle = (Animation){0};
static const Animation attack = (Animation){0};
static const Animation dodge = (Animation){0};

static Animation animations[] = {walk};

static bool is_direction_left = false;

void anim_init(Player *player, Animation *anim) {
  anim->counter = 0;
  anim->texture = LoadTexture(anim->path);
  // segfaults here
  printf("\n\t animation loaded ID is : %d \n \t", anim->texture.id);
  anim->texture.width *= player->size.x;
  anim->texture.height *= player->size.y;
  anim->frame.x = 0;
  anim->frame.y = 0;
  anim->frame.width = (float)anim->texture.width / anim->length;
  anim->frame.height = anim->texture.height;
}

Player *player_init(Vector2 size, Color color) {
  Player *player = malloc(sizeof(Player));
  player->size = size;
  player->color = color;
  player->animations_length = sizeof(animations) / sizeof(animations[0]);
  player->animations = malloc(player->animations_length * sizeof(Animation));
  player->animations = animations;
  player->curr_anim_index = 0;
  anim_init(player, &animations[0]);
  return player;
}

void player_deconstruct(Player *player) {
  for (int i = 0; i < player->animations_length; i++) {
    free((void *)player->animations[i].path);
  }
  free(player->animations);
  if (player->state.extra_data)
    free(player->state.extra_data);
  free(player);
}

void player_adjust_state(State state);

void handle_animations(Player *player) {
  Animation *animation = &(player->animations[player->curr_anim_index]);
  animation->counter += 1;
  if (animation->counter >= 60 / .3) {
    animation->curr_frame += 1;
    animation->counter = 0;
    if (animation->curr_frame > animation->length - 1)
      animation->curr_frame = 0;
    animation->frame.x = (float)animation->curr_frame *
                         (float)animation->texture.width / animation->length;
  }
  DrawTextureRec(animation->texture, animation->frame, player->position,
                 player->color);
}

void flip(Player *player, bool is_left) {
  if (is_left)
    player->animations[player->curr_anim_index].frame.width =
        -player->animations[player->curr_anim_index].frame.width;
  else
    player->animations[player->curr_anim_index].frame.width =
        fabs(player->animations[player->curr_anim_index].frame.width);
}

void renderer_init(const int width, const int height, const char *title) {
  InitWindow(width, height, title);
  SetTargetFPS(FPS_CAP);
}

void handle_input(Player *player) {
  if (IsKeyDown(KEY_Q)) {
    if (is_direction_left)
      flip(player, true);
    is_direction_left = false;
    player->position = Vector2Add(player->position, (Vector2){.x = -.1, 0});
  }
  if (IsKeyDown(KEY_D)) {
    if (!is_direction_left)
      flip(player, false);
    is_direction_left = true;
    player->position = Vector2Add(player->position, (Vector2){.x = .1, 0});
  }
}

void renderer_render(const int width, const int height, Player *player) {
  Animation animation = player->animations[player->curr_anim_index];
  player->position = (Vector2){.x = width / 2.0, .y = height / 2.0};
  printf("\t\n ID %d\n", animation.texture.id);
  printf("%d %d %f %f \n", animation.texture.width, animation.texture.height,
         animation.frame.height, animation.frame.width);
  while (!WindowShouldClose()) {
    BeginDrawing();
    handle_input(player);
    ClearBackground(LIGHTGRAY);
    handle_animations(player);
    EndDrawing();
  }
  player_deconstruct(player);
  CloseWindow();
}
