#include "../libs/dynarr.h"
#include <stdint.h>

// typedef struct {
//   uint16_t e_size; 4
//   uint16_t length; 4
//   uint16_t occupied_length; 4
//   uint16_t step;
//   void *items; // 8 bytes
// } DynArray;

DynArray *da_init(uint16_t e_size, void *items, uint16_t occupied_length,
                  uint16_t step) {
  DynArray *array =
      (DynArray *)malloc(sizeof(DynArray) + (step + occupied_length) * e_size);
  array->occupied_length = occupied_length;
  array->e_size = e_size;
  array->occupied_length = occupied_length;
  array->step = step;
  array->items = malloc((step + occupied_length) * e_size);
  if (items == NULL) {
    puts("item is null");
    memset(array->items, 0, step * e_size);
  } else {
    puts("item is not null");
    memset(array->items, 0, (occupied_length + step) * e_size);
    memcpy(array->items, items, occupied_length * e_size);
  }
  return array;
}

bool da_find(DynArray *array, void *data) {
  if (array->items == NULL)
    return false;
  if (!array->occupied_length)
    return false;
  for (int i = 0; i < array->occupied_length; i++) {
    if (memcmp(&((char *)array->items)[i], (char *)data, array->e_size) == 0)
      return i;
    return false;
  }
  return false;
}

void da_append(DynArray *array, void *data) {
  if (array->occupied_length == array->step) {
    size_t new_size = (array->occupied_length + array->step) * array->e_size;
    array->items = (void *)realloc(array->items, new_size);
    if (array->items == NULL) {
      printf("BUY MORE RAM!\n");
      exit(EXIT_FAILURE);
    }
    array->occupied_length += array->step;
  }
  memcpy(((char *)array->items) + array->occupied_length++ * array->e_size,
         data, array->e_size);
}

void da_remove(DynArray *array, void *data) {
  if (array->items == NULL)
    return;
  if (!array->occupied_length)
    return;
  for (int i = 0; i < array->occupied_length; i++) {
    if (memcmp(&((char *)array->items)[i], (char *)data, array->e_size) == 0) {
      for (int y = i; y < array->occupied_length; y++)
        array[y] = array[y + 1];
      break;
    }
  }
}

void da_free(DynArray *array) {
  free(array->items);
  array->items = NULL;
  free(array);
}
