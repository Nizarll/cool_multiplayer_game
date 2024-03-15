#include <stdio.h>

#define let __auto_type

int main(void) {
  auto a = 1;
  let b = "hello world";
  let c = 'c';
  printf("%f\n", a);
  return 0;
}
