#include "index.h"

thread_t *
thread_start(thread_fn_t *thread_fn, void *arg) {
    thread_t *self = new(thread_t);
    int ok = pthread_create(&self->pthread, NULL, thread_fn, arg);
    assert(ok == 0);
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
