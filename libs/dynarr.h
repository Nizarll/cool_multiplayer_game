#ifndef DYNARR_H
#define DYNARR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *items;
  size_t e_size;
  size_t length;
  size_t occupied_length;
  size_t step;
} DynArray;

DynArray *da_init(void *items, size_t e_size, size_t length,
                  size_t occupied_length, size_t step);
bool da_find(DynArray *array, void *data);
void da_append(DynArray *array, void *data);
void da_remove(DynArray *array, void *data);
void da_free(DynArray *array);

#endif // DYNARR_H
