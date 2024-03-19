#include "../libs/dynarr.h"

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
  for (int i = 0; i < array->occupied_length; i++) {
    if (memcmp(&array->items[i], data, array->e_size) == 0) {
      for (int y = i; y < array->occupied_length; y++) {
        array[y] = array[y + 1];
      }
      break;
    }
  }
}

void da_free(DynArray *array) {
  free(array->items);
  array->items = NULL;
  free(array);
}
