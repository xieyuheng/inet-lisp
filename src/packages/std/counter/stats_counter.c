#include "index.h"

stats_counter_t *
stats_counter_new(size_t size) {
    stats_counter_t *self = new(stats_counter_t);
    self->size = size;
    self->count_pointer_array = array_new(size);
    return self;
}
