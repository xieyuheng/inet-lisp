#include "index.h"

typedef _Atomic int64_t atomic_int64_t;

struct stats_counter_t {
    size_t size;
    array_t *counter_array;
};

stats_counter_t *
stats_counter_new(size_t size) {
    stats_counter_t *self = new(stats_counter_t);
    self->size = size;
    self->counter_array = array_new_with(size, destroy);
    for (size_t i = 0; i < size; i++) {
        atomic_int64_t *counter = new_page_aligned(atomic_int64_t);
        atomic_init(counter, 0);
        array_push(self->counter_array, counter);
    }

    return self;
}

void
stats_counter_destroy(stats_counter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    stats_counter_t *self = *self_pointer;
    array_destroy(&self->counter_array);
    free(self);
    *self_pointer = NULL;
}

void
stats_counter_per_thread_add1(stats_counter_t *self, size_t id) {
    atomic_int64_t *counter = array_get(self->counter_array, id);
    int64_t count = relaxed_load(counter);
    relaxed_store(counter, count + 1);
}

void
stats_counter_per_thread_sub1(stats_counter_t *self, size_t id) {
    atomic_int64_t *counter = array_get(self->counter_array, id);
    int64_t count = relaxed_load(counter);
    relaxed_store(counter, count - 1);
}

int64_t
stats_counter_total(stats_counter_t *self) {
    size_t total = 0;
    for (size_t i = 0; i < self->size; i++) {
        atomic_int64_t *counter = array_get(self->counter_array, i);
        size_t count = relaxed_load(counter);
        total += count;
    }

    return total;
}

bool
stats_counter_total_is_zero(stats_counter_t *self) {
    return stats_counter_total(self) == 0;
}
