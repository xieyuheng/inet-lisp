#pragma once

struct scheduler_t {
    mod_t *mod;
    node_allocator_t *node_allocator;
    array_t *worker_array;
    thread_pool_t *worker_thread_pool;
    stats_counter_t *stats_counter;
};

scheduler_t *scheduler_new(mod_t *mod, node_allocator_t *node_allocator, size_t worker_count);
void scheduler_destroy(scheduler_t **self_pointer);

size_t scheduler_worker_count(scheduler_t *self);
worker_t *scheduler_get_worker(scheduler_t *self, size_t id);
void scheduler_put_worker(scheduler_t *self, size_t id, worker_t *worker);

void scheduler_start(scheduler_t *self, thread_fn_t *worker_thread_fn);
void scheduler_wait(scheduler_t *self);

void scheduler_task_count_add1(scheduler_t *self, size_t id);
void scheduler_task_count_sub1(scheduler_t *self, size_t id);
bool scheduler_no_more_tasks(scheduler_t *self);
