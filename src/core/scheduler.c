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
        worker->id = i;
        array_push(self->worker_array, worker);
    }

    self->worker_thread_pool = thread_pool_new();
    self->stats_counter = stats_counter_new(worker_count);
    return self;
}

void
scheduler_destroy(scheduler_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    scheduler_t *self = *self_pointer;
    array_destroy(&self->worker_array);
    stats_counter_destroy(&self->stats_counter);
    thread_pool_destroy(&self->worker_thread_pool);
    free(self);
    *self_pointer = NULL;
}

size_t
scheduler_worker_count(scheduler_t *self) {
    return array_length(self->worker_array);
}

worker_t *
scheduler_get_worker(scheduler_t *self, size_t id) {
    return array_get(self->worker_array, id);
}

void
scheduler_set_worker(scheduler_t *self, size_t id, worker_t *worker) {
    array_set(self->worker_array, id, worker);
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
scheduler_task_count_add1(scheduler_t *self, size_t id) {
    stats_counter_per_thread_add1(self->stats_counter, id);
}

void
scheduler_task_count_sub1(scheduler_t *self, size_t id) {
    stats_counter_per_thread_sub1(self->stats_counter, id);
}

bool
scheduler_no_more_tasks(scheduler_t *self) {
    return stats_counter_total_is_zero(self->stats_counter);
}
