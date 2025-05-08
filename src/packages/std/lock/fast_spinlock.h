#pragma once

struct fast_spinlock_t {
    atomic_bool atomic_is_locked;
};

fast_spinlock_t *fast_spinlock_new(void);
void fast_spinlock_destroy(fast_spinlock_t **self_pointer);

inline void
fast_spinlock_lock(fast_spinlock_t *self) {
    while (atomic_load_explicit(
               &self->atomic_is_locked,
               memory_order_relaxed) ||
           atomic_exchange_explicit(
               &self->atomic_is_locked,
               true,
               memory_order_acquire))
    {
        time_sleep_nanosecond(1);
    }
}

inline bool
fast_spinlock_try_lock(fast_spinlock_t *self) {
    return !(atomic_load_explicit(
                 &self->atomic_is_locked,
                 memory_order_relaxed) ||
             atomic_exchange_explicit(
                 &self->atomic_is_locked,
                 true,
                 memory_order_acquire));
}

inline void
fast_spinlock_unlock(fast_spinlock_t *self) {
    atomic_store_explicit(
        &self->atomic_is_locked,
        false,
        memory_order_release);
}
