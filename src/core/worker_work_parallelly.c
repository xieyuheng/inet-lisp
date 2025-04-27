#include "index.h"

inline static task_t *
worker_steal_task(worker_t *worker) {
    scheduler_t *scheduler = worker->scheduler;
    size_t worker_count = scheduler_worker_count(scheduler);

    while (!scheduler_no_more_tasks(scheduler)) {
        size_t victim_id = ++worker->victim_cursor % worker_count;
        if (victim_id == worker->worker_id)
            victim_id = ++worker->victim_cursor % worker_count;

        worker_t *victim = scheduler_get_worker(scheduler, victim_id);
        task_t *task = deque_pop_front(victim->task_deque);
        if (task) return task;
    }

    return NULL;
}

static void *
worker_thread_fn(void *arg) {
    worker_t *worker = arg;
    scheduler_t *scheduler = worker->scheduler;

    while (!scheduler_no_more_tasks(scheduler)) {
        task_t *task = deque_pop_front(worker->task_deque);

#if DEBUG_WORK_STEALING_DISABLED
        (void) worker_steal_task;
#else
        if (!task) task = worker_steal_task(worker);
#endif
        if (!task) return NULL;

        worker_handle_task(worker, task);
        atomic_fetch_sub_explicit(
            &scheduler->atomic_task_count,
            1,
            memory_order_release);
    }

    return NULL;
}

static void
scheduler_prepare(scheduler_t *scheduler, deque_t *init_task_deque) {
    size_t cursor = 0;
    while (!deque_is_empty(init_task_deque)) {
        task_t *task = deque_pop_front(init_task_deque);
        size_t index = cursor % scheduler_worker_count(scheduler);
        worker_t *worker = scheduler_get_worker(scheduler, index);
        deque_push_back(worker->task_deque, task);
        atomic_fetch_add_explicit(
            &scheduler->atomic_task_count,
            1,
            memory_order_release);
        cursor++;
    }
}

void
worker_work_parallelly(worker_t *worker) {
    size_t processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    size_t worker_count = processor_count - 1;
    scheduler_t *scheduler = scheduler_new(worker->mod, worker->node_allocator, worker_count);
    scheduler_prepare(scheduler, worker->task_deque);
    scheduler_start(scheduler, worker_thread_fn);
    scheduler_wait(scheduler);
    scheduler_destroy(&scheduler);
}
