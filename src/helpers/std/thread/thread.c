#include "index.h"

static void *
pthread_start_fn(void *arg) {
    thread_t *thread = arg;
    thread->thread_fn(thread);
    return NULL;
}

thread_t *
thread_start(thread_fn_t *thread_fn, void *arg) {
    thread_t *self = new(thread_t);
    self->thread_fn = thread_fn;
    self->arg = arg;
    int ok = pthread_create(&self->pthread, NULL, pthread_start_fn, self);
    assert(ok == 0);
    return self;
}

void
thread_join(thread_t *self) {
    void *result;
    int ok = pthread_join(self->pthread, &result);
    assert(ok == 0);
    assert(result == NULL);
    free(self);
}
