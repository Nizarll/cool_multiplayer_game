#ifndef DYNARR_H
#define DYNARR_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint16_t e_size;
  uint16_t occupied_length;
  void *items;
  uint16_t step;
} DynArray;

DynArray *da_init(uint16_t e_size, void *items = NULL,
                  uint16_t occupied_length = 0, uint16_t step = 100);
bool da_find(DynArray *array, void *data);
void da_append(DynArray *array, void *data);
void da_remove(DynArray *array, void *data);
void da_free(DynArray *array);

#endif // DYNARR_H
