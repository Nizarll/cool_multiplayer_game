#include "../libs/ease.h"

static const float back_c1 = 1.70158;
static const float back_c2 = back_c1 * 1.525;

typedef struct Ease {
  bool finished;
} Ease;

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
