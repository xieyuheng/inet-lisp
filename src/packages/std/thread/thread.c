#include "index.h"

struct thread_t {
    pthread_t pthread;
    bool is_in_pool;
    size_t id;
};

thread_t *
thread_start(thread_fn_t *thread_fn, void *arg) {
    thread_t *self = new(thread_t);
    int ok = pthread_create(&self->pthread, NULL, thread_fn, arg);
    assert(ok == 0);
    self->is_in_pool = false;
    self->id = 0;
    return self;
}

void *
thread_join(thread_t *self) {
    void *result;
    int ok = pthread_join(self->pthread, &result);
    assert(ok == 0);
    free(self);
    return result;
}

bool
thread_is_in_pool(thread_t *self) {
    return self->is_in_pool;
}
