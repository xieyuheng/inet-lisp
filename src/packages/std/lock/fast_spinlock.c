#include "index.h"

fast_spinlock_t *
fast_spinlock_new(void) {
    fast_spinlock_t *self = new(fast_spinlock_t);
    atomic_init(&self->atomic_is_locked, false);
    return self;
}

void
fast_spinlock_destroy(fast_spinlock_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    fast_spinlock_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

void fast_spinlock_lock(fast_spinlock_t *self);
bool fast_spinlock_try_lock(fast_spinlock_t *self);
void fast_spinlock_unlock(fast_spinlock_t *self);
