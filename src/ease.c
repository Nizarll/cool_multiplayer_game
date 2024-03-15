#include "../libs/ease.h"

static const float back_c1 = 1.70158;
static const float back_c2 = back_c1 * 1.525;

typedef struct Ease {
  bool finished;
} Ease;

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
float ease_inout_circle(float a); // unimplemented
float ease_inout_quart(float a);
float ease_inout_elastic(float a);
float ease_inout_sine(float a) { return -(cosf(PI * a) - 1) / 2; }
float ease_inout_back(float a) {
  if (a < .5) {
    return (powf(2 * a, 2) * ((back_c2 + 1) * 2 * a - back_c2)) / 2;
  } else {
    return (powf(2 * a - 2, 2) * ((back_c2 + 1) * (a * 2 - 2) + back_c2) + 2);
  }
}
float ease_inout_circ(float a) {
  if (a < .5) {
    return (1 - sqrtf(1 - powf(2 * a, 2))) / 2;
  } else {
    return (sqrtf(1 - powf(-2 * a + 2, 2)) + 1) / 2;
  }
}

typedef struct {
  Kind kind;
  Vector2 v2_a;
  Vector2 v2_b;
  Ease ease;
} Params;

void ease_v2(void *args) {
  Kind kind = (Kind)((Params *)args)->kind;
  Ease ease = (Ease)((Params *)args)->ease;
  Vector2 v2_a = (Vector2)((Params *)args)->v2_a;
  Vector2 v2_b = (Vector2)((Params *)args)->v2_b;
  while (v2_a.x != v2_b.x && v2_a.y != v2_b.y) {
    switch (kind) {
    case eIOSINE:
      ease_inout_sine(v2_a.x);
      ease_inout_sine(v2_a.y);
      break;
    case eIOELASTIC:
      ease_inout_elastic(v2_a.x);
      ease_inout_elastic(v2_a.y);
      break;
    case eIOBACK:
      ease_inout_circle(v2_a.x);
      ease_inout_circle(v2_a.y);
      break;
    case eIOQUART:
      ease_inout_quart(v2_a.x);
      ease_inout_quart(v2_a.y);
      break;
    default:
      exit(EXIT_FAILURE);
      break;
    }
    msleep(50);
  }
  ease.finished = true;
}

void spawn_func(void (*f)(Vector2), float *arg) {
  pthread_t thread_id, thread_ret;
  pthread_create(&thread_id, NULL, (void *)f, arg);
  pthread_join(thread_id, NULL);
}

Ease ease(Kind kind, Vector2 v2_a, Vector2 v2_b) {
  Ease ease = {.finished = false};
  Params *args = malloc(sizeof(Params));
  args->v2_a = v2_a;
  args->v2_b = v2_b;
  args->kind = kind;
  spawn_func((void *)ease_v2, (void *)args);
  return ease;
}
