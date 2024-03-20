#include "../libs/renderer.h"
#include "../libs/ease.h"

#include <pthread.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <time.h>

#define FPS_CAP (4999)

typedef enum {
  eLEFT,
  eRIGHT,
} Direction;

typedef struct {
  Player *player;
  bool is_left;
} Args;

static time_t deltatime = 0;

const Animation walk = (Animation){
    .path = "./resources/character_walk_cycle.png",
    .length = 4,
    .speed = 1,
};

static const Animation idle = (Animation){
    .path = "./resources/character.png",
    .length = 1,
    .speed = 1,
};

static const Animation attack = (Animation){0};
static const Animation dash = (Animation){0};

static Animation animations[] = {
    [eWALK] = walk, [eIDLE] = idle, [eJUMP] = idle,
    // [S_M1] = walk,
    // [S_DASH] = walk,
};

static bool is_direction_left = false;
static bool can_flip = true;

int msleep(long msec) {
  struct timespec ts;
  int res;
  if (msec < 0) {
    errno = EINVAL;
    return -1;
  }
  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;
  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);
  return res;
}

void anim_init(Player *player, Animation *anim) {
  anim->counter = 0;
  anim->texture = LoadTexture(anim->path);
  anim->texture.width *= player->size.x;
  anim->texture.height *= player->size.y;
  anim->frame.x = 0;
  anim->frame.y = 0;
  anim->frame.width = anim->texture.width;
  anim->frame.height = (float)anim->texture.height / anim->length;
}

// typedef struct Player {
//   Color color;
//   uint8_t rotation;
//   Vector2 position;
//   Vector2 size;
//   DynArray *states;
//   RigidBody *rb;
//   //
//   Animation *animations;
//   size_t animations_length;
//   size_t curr_anim_index;
// } Player;

// typedef struct RigidBody {
//   Vector2 *hitbox;
//   DynArray *forces;
// } RigidBody;

Player *player_init(Vector2 size, Color color) {
  Player *player = (Player *)malloc(sizeof(Player));
  RigidBody *rb = (RigidBody *)malloc(sizeof(RigidBody));
  DynArray *forces = (DynArray *)da_init(sizeof(Vector2));
  DynArray *states = (DynArray *)da_init(sizeof(State));
  printf("\t\tINFO: %d %d", forces->occupied_length, states->occupied_length);
  rb->forces = forces;
  player->rb = rb;
  player->states = states;
  player->size = size;
  player->color = color;
  player->rotation = 0;
  player->animations_length = sizeof(animations) / sizeof(animations[0]);
  player->animations =
      (Animation *)malloc(player->animations_length * sizeof(Animation));
  player->animations = (Animation *)animations;
  player->curr_anim_index = 0;
  for (int i = 0; i < player->animations_length; i++) {
    anim_init(player, &animations[i]);
  }
  return player;
}

void player_deconstruct(Player *player) {
  for (int i = 0; i < player->animations_length; i++) {
    free((void *)player->animations[i].path);
  }
  free(player->animations);
  da_free(player->states);
  free(player);
}

void handle_animations(Player *player) {
  Animation *animation = &(player->animations[has_state(player, eWALK) || 0]);
  Vector2 origin = {(float)animation->frame.width / 2,
                    (float)animation->frame.height / 2};
  if (animation && has_state(player, eWALK)) {
    animation->counter += 1;
    if (animation->counter >= 60 / .3) {
      animation->curr_frame += 1;
      animation->counter = 0;
      if (animation->curr_frame > animation->length - 1)
        animation->curr_frame = 0;
      animation->frame.y = (float)animation->curr_frame *
                           (float)animation->texture.height / animation->length;
    }
  }
  DrawTexturePro(animation->texture, animation->frame,
                 (Rectangle){player->position.x, player->position.y,
                             animation->frame.width, animation->frame.height},
                 origin, 0.0f, player->color);
}

float lerp(float a, float b, float t) { return a + t * (b - a); }

void *animate_flip(void *args) {
  bool is_left = ((Args *)args)->is_left;
  Player *player = ((Args *)args)->player;
  float text_width, frame_width;
  can_flip = false;
  if (is_left) {
    text_width =
        -fabs((float)player->animations[player->curr_anim_index].texture.width);
  } else {
    text_width =
        fabs((float)player->animations[player->curr_anim_index].texture.width);
  }
  frame_width = (float)player->animations[player->curr_anim_index].frame.width;
  float i = 0;
  while (i < 1) {
    i = i < .2 ? i + .0035 : 1;
    if (i < .11) {
      player->animations[player->curr_anim_index].frame.width =
          lerp(player->animations[player->curr_anim_index].frame.width, 0, i);
    } else {
      player->animations[player->curr_anim_index].frame.width =
          lerp(player->animations[player->curr_anim_index].frame.width,
               frame_width, i);
    }
    player->animations[player->curr_anim_index].texture.width =
        lerp(player->animations[player->curr_anim_index].texture.width,
             text_width, i);
    msleep(5);
  }
  can_flip = true;
  pthread_exit(NULL);
}
// summon flip animation in a new thread
void flip(void *args) {
  pthread_t t_id;
  pthread_create(&t_id, NULL, animate_flip, args);
  pthread_join(t_id, NULL);
}

void *animate_jump(void *args) {
  // Player *player = (Player *)args;
  pthread_exit(NULL);
}

void jump(Player *player) {
  pthread_t t_id;
  pthread_create(&t_id, NULL, animate_jump, player);
}

void renderer_init(const int width, const int height, const char *title) {
  InitWindow(width, height, title);
  SetTargetFPS(FPS_CAP);
}

void handle_input(Player *player) {
  if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) &&
      can_transition_state(player, eWALK)) {
    int *direction;
    *direction = IsKeyDown(KEY_A) ? eLEFT : eRIGHT;
    // da_append(player->states, &(State){
    //                               .kind = eWALK,
    //                               .data = (void *)direction,
    //                           });
  }
  if ((IsKeyDown(KEY_SPACE) && can_transition_state(player, eJUMP))) {
    // da_append(player->states, &(State){
    //                               .kind = eJUMP,
    //                           });
  }
}

void handle_physics(DynArray *arr) {
  printf("%d", arr->occupied_length);
  for (int i = 0; i < arr->occupied_length; i++) {
    Player player = *((Player **)arr->items)[i];
    puts("\n\tINFO: 221");
    printf("\n\n\t%d", player.rb->forces->occupied_length);

    //  for (int y = 0; y < arr->occupied_length; y++) {
    //    player->position = (Vector2){
    //        .x = player->position.x + ((Vector2
    //        *)player->rb->forces->items)[y].x, .y = player->position.y +
    //        ((Vector2 *)player->rb->forces->items)[y].y,
    //    };
    // }
  }
}

void renderer_render(const int width, const int height, Player *player) {
  DynArray *players = (DynArray *)da_init(sizeof(Player *));
  da_append(players, player);
  printf("\t\n%d \t\n", players->occupied_length);
  player->position = (Vector2){.x = width / 2.0f, .y = height / 2.0f};
  while (!WindowShouldClose()) {
    handle_physics(players);
    handle_input(player);
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    DrawText(TextFormat("Fps : %03i", GetFPS()), GetScreenWidth() - 150, 20, 20,
             BLACK);
    handle_animations(player);
    EndDrawing();
    deltatime = time(NULL) - deltatime;
  }
  da_free(players);
  player_deconstruct(player);
  CloseWindow();
}
