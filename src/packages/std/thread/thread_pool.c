#include "index.h"

struct thread_pool_t {
    array_t *thread_array;
};

thread_pool_t *
thread_pool_new(void) {
    thread_pool_t *self = new(thread_pool_t);
    self->thread_array = array_new_auto();
    return self;
}

void
thread_pool_destroy(thread_pool_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    thread_pool_t *self = *self_pointer;
    array_destroy(&self->thread_array);
    free(self);
    *self_pointer = NULL;
}
