#include "../libs/renderer.h"
#include "../libs/ease.h"

#include <pthread.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <time.h>

#define FPS_CAP (4999)

typedef struct {
  Player *player;
  bool is_left;
} Args;

typedef struct {
  const char *type;
  union {
    Player *player;
    // other types
  } entity;
} RigidBody;

typedef struct {
  RigidBody *rbs;
  size_t length;
} RigidBodyArr;

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
    [S_WALK] = walk, [S_IDLE] = idle, [S_JUMP] = idle,
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

Player *player_init(Vector2 size, Color color) {
  Player *player = malloc(sizeof(Player));
  player->size = size;
  player->color = color;
  player->rotation = 0;
  player->animations_length = sizeof(animations) / sizeof(animations[0]);
  player->animations = malloc(player->animations_length * sizeof(Animation));
  player->animations = animations;
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
  if (player->state.extra_data)
    free(player->state.extra_data);
  free(player);
}

void handle_animations(Player *player) {
  Animation *animation = &(player->animations[player->state.kind]);
  Vector2 origin = {(float)animation->frame.width / 2,
                    (float)animation->frame.height / 2};
  if (animation && player->state.kind == S_WALK) {
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

void animate_flip(void *args) {
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
}
// summon flip animation in a new thread
void flip(void *args) {
  pthread_t t_id;
  pthread_create(&t_id, NULL, (void *)animate_flip, args);
  pthread_join(t_id, NULL);
}

void animate_jump(void *args) {
  Player *player = (Player *)args;
  for (int i = 0; i < 10; i++) {
    printf("hi\n");
    player->position = Vector2Add(player->position, (Vector2){
                                                        .x = 0,
                                                        .y = -10,
                                                    });
    msleep(10);
  }
}

void jump(Player *player) {
  pthread_t t_id;
  pthread_create(&t_id, NULL, (void *)animate_jump, player);
}

void renderer_init(const int width, const int height, const char *title) {
  InitWindow(width, height, title);
  SetTargetFPS(FPS_CAP);
}

void handle_input(Player *player) {
  if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A) && player->state.kind != S_JUMP)
    player->state = (State){.kind = S_IDLE};
  if (IsKeyDown(KEY_Q)) {
    if (!is_direction_left && can_flip) {
      pthread_t fthread_id;
      Args *flip_args = malloc(sizeof(Args));
      flip_args->player = player;
      flip_args->is_left = is_direction_left = true;
      pthread_create(&fthread_id, NULL, (void *)flip, flip_args);
    }
    player->state = (State){
        .kind = S_WALK,
    };
    player->position = Vector2Add(player->position, (Vector2){.x = -.1, 0});
  } else if (IsKeyDown(KEY_D)) {
    if (is_direction_left && can_flip) {
      pthread_t fthread_id;
      Args *flip_args = malloc(sizeof(Args));
      flip_args->player = player;
      flip_args->is_left = is_direction_left = false;
      pthread_create(&fthread_id, NULL, (void *)flip, flip_args);
    }
    player->state = (State){
        .kind = S_WALK,
    };
    player->position = Vector2Add(player->position, (Vector2){.x = .1, 0});
  }
  if (IsKeyDown(KEY_SPACE) && player->state.kind != S_JUMP) {
    player->state = (State){.kind = S_JUMP};
    jump(player);
  }
}

// void do_physic(void *pp);

void handle_physics(RigidBodyArr *arr) {
  for (int i = 0; i < arr->length; i++) {
    RigidBody rb = arr->rbs[i];
    if (strncmp(rb.type, "player", strlen(rb.type)) == 0) {
      if (rb.entity.player->position.y >= 400)
        continue;
      rb.entity.player->position =
          Vector2Add(rb.entity.player->position, (Vector2){
                                                     .x = 0,
                                                     .y = .1,
                                                 });
      //  check for collisions here
    }
  }
}

void physics_deconstruct(RigidBodyArr *arr) {
  free(arr->rbs);
  free(arr);
}

void renderer_render(const int width, const int height, Player *player) {

  // Initialize the array of rigidbody objects although that should be done on
  // the server lol

  RigidBodyArr *arr = (RigidBodyArr *)malloc(sizeof(RigidBodyArr));
  RigidBody *rbs = (RigidBody *)malloc(1 * sizeof(RigidBody));
  arr->length = 1;
  rbs[0] = (RigidBody){
      .type = "player",
      .entity.player = player,
  };
  arr->rbs = rbs;
  Animation animation = player->animations[player->curr_anim_index];
  player->position = (Vector2){.x = width / 2.0, .y = height / 2.0};

  while (!WindowShouldClose()) {
    handle_physics(arr);
    handle_input(player);
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    DrawText(TextFormat("Fps : %03i", GetFPS()), GetScreenWidth() - 150, 20, 20,
             BLACK);
    handle_animations(player);
    EndDrawing();
    deltatime = time(NULL) - deltatime;
  }
  physics_deconstruct(arr);
  player_deconstruct(player);
  CloseWindow();
}
