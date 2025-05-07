#include "index.h"

typedef pthread_t tid_t;

struct thread_t {
    tid_t tid;
};

thread_t *
thread_start(thread_fn_t *thread_fn, void *arg) {
    thread_t *self = new(thread_t);
    int ok = pthread_create(&self->tid, NULL, thread_fn, arg);
    assert(ok == 0);
    return self;
}

void *
thread_join(thread_t *self) {
    void *result;
    int ok = pthread_join(self->tid, &result);
    assert(ok == 0);
    free(self);
    return result;
}
