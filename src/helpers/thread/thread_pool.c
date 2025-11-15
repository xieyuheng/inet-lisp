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

size_t
thread_pool_start(thread_pool_t *self, thread_fn_t *thread_fn, void *arg) {
    thread_t *thread = thread_start(thread_fn, arg);
    size_t id = array_length(self->thread_array);
    array_set(self->thread_array, id, thread);
    thread->thread_pool = self;
    thread->id = id;
    return id;
}

void
thread_pool_join(thread_pool_t *self, size_t id) {
    thread_t *thread = array_get(self->thread_array, id);
    assert(thread);
    thread_join(thread);
    array_set(self->thread_array, id, NULL);
}

void
thread_pool_join_all(thread_pool_t *self) {
    size_t length = array_length(self->thread_array);
    for (size_t i = 0; i < length; i++) {
        thread_t *thread = array_get(self->thread_array, i);
        if (thread) thread_join(thread);
    }
}
