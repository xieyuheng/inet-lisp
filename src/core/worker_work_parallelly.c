#include "index.h"

inline static task_t *
worker_steal_task(worker_t *worker) {
    scheduler_t *scheduler = worker->scheduler;
    size_t worker_count = scheduler_worker_count(scheduler);

    while (!scheduler_no_more_tasks(scheduler)) {
        size_t victim_id = ++worker->victim_cursor % worker_count;
        if (victim_id == worker->id)
            victim_id = ++worker->victim_cursor % worker_count;

        worker_t *victim = scheduler_get_worker(scheduler, victim_id);
        task_t *task = deque_pop_front(victim->task_deque);
        if (task) {
            return task;
        }
    }

    return NULL;
}

static void
worker_thread_fn(thread_t *thread) {
    worker_t *worker = thread->arg;
    scheduler_t *scheduler = worker->scheduler;

    while (!scheduler_no_more_tasks(scheduler)) {
        task_t *task = worker_next_task(worker);

#if DEBUG_WORK_STEALING_DISABLED
        (void) worker_steal_task;
#else
        if (!task) task = worker_steal_task(worker);
#endif
        if (!task) return;

        worker_handle_task(worker, task);
        scheduler_task_count_sub1(scheduler, thread->id);
    }
}

static void
scheduler_prepare(scheduler_t *scheduler, worker_t *loader_worker) {
    size_t cursor = 0;
    while (true) {
        task_t *task = worker_next_task(loader_worker);
        if (!task) return;
        size_t index = cursor % scheduler_worker_count(scheduler);
        worker_t *worker = scheduler_get_worker(scheduler, index);
        worker_add_task(worker, task);
        cursor++;
    }
}

void
worker_work_parallelly(worker_t *worker) {
    size_t processor_count = sysconf(_SC_NPROCESSORS_ONLN);
    size_t worker_count = processor_count - 1;
    scheduler_t *scheduler = scheduler_new(worker->mod, worker->node_allocator, worker_count);
    scheduler_prepare(scheduler, worker);
    scheduler_start(scheduler, worker_thread_fn);
    scheduler_wait(scheduler);
    scheduler_destroy(&scheduler);
}
