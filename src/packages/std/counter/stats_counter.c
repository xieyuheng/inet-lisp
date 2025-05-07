#include "index.h"

stats_counter_t *
stats_counter_new(size_t size) {
    stats_counter_t *self = new(stats_counter_t);
    self->size = size;
    self->count_pointer_array = array_new(size);
    return self;
}

void
stats_counter_destroy(stats_counter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    stats_counter_t *self = *self_pointer;
    array_destroy(&self->count_pointer_array);
    free(self);
    *self_pointer = NULL;
}
