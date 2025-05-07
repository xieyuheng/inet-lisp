#include "index.h"

task_t *
worker_offer_task(worker_t *worker) {
    fast_spinlock_lock(worker->task_queue_front_lock);
    task_t *task = queue_pop_front(worker->task_queue);
    fast_spinlock_unlock(worker->task_queue_front_lock);
    return task;
}
