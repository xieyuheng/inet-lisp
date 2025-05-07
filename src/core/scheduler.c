#include "index.h"

scheduler_t *
scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count) {
    scheduler_t *self = new(scheduler_t);
    self->mod = mod;
    self->node_allocator = node_allocator;
    self->worker_array = array_new_auto_with((destroy_fn_t *) worker_destroy);
    for (size_t i = 0; i < worker_count; i++) {
        worker_t *worker = worker_new(mod, node_allocator);
        worker->scheduler = self;
        worker->worker_id = i;
        array_push(self->worker_array, worker);
    }

    self->worker_thread_pool = thread_pool_new();
    atomic_init(&self->atomic_task_count, 0);
    return self;
}

void
scheduler_destroy(scheduler_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    scheduler_t *self = *self_pointer;
    array_destroy(&self->worker_array);
    thread_pool_destroy(&self->worker_thread_pool);
    free(self);
    *self_pointer = NULL;
}

size_t
scheduler_worker_count(scheduler_t *self) {
    return array_length(self->worker_array);
}

worker_t *
scheduler_get_worker(scheduler_t *self, size_t worker_id) {
    return array_get(self->worker_array, worker_id);
}

void
scheduler_set_worker(scheduler_t *self, size_t worker_id, worker_t *worker) {
    array_set(self->worker_array, worker_id, worker);
}

void
scheduler_start(scheduler_t *self, thread_fn_t *worker_thread_fn) {
    for (size_t i = 0; i < array_length(self->worker_array); i++) {
        worker_t *worker = array_get(self->worker_array, i);
        thread_pool_start(self->worker_thread_pool, worker_thread_fn, worker);
    }
}

void
scheduler_wait(scheduler_t *self) {
    thread_pool_join_all(self->worker_thread_pool);
}

void
scheduler_task_count_add1(scheduler_t *self) {
    atomic_fetch_add_explicit(
        &self->atomic_task_count,
        1,
        memory_order_release);
}

void
scheduler_task_count_sub1(scheduler_t *self) {
    atomic_fetch_sub_explicit(
        &self->atomic_task_count,
        1,
        memory_order_release);
}

bool
scheduler_no_more_tasks(scheduler_t *self) {
    size_t task_count = atomic_load_explicit(
        &self->atomic_task_count,
        memory_order_acquire);

    return task_count == 0;
}
