#include "../libs/dynarr.h"

DynArray *da_init(void *items, size_t e_size, size_t length,
                  size_t occupied_length, size_t step) {
  DynArray *array = (DynArray *)malloc(sizeof(DynArray));
  array->e_size = e_size;
  array->length = length;
  array->occupied_length = occupied_length;
  array->step = step;
  array->items = (void *)malloc(array->e_size * array->length);
  return array;
}

bool da_find(DynArray *array, void *data) {
  if (array->items == NULL)
    return false;
  if (!array->occupied_length)
    return false;
  for (int i = 0; i < array->occupied_length; i++) {
    if (memcmp(&array->items[i], data, array->e_size) == 0)
      return i;
  }
  return false;
}

void da_append(DynArray *array, void *data) {
  if (array->occupied_length == array->length) {
    size_t new_size = (array->length + array->step) * array->e_size;
    array->items = (void *)realloc(array->items, new_size);
    if (array->items) {
      printf("BUY MORE RAM!\n");
      exit(EXIT_FAILURE);
    }
    array->length += array->step;
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
    if (memcmp(&array->items[i], data, array->e_size) == 0) {
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
