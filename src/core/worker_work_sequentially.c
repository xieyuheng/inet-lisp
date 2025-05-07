#include "index.h"

void
worker_work_sequentially(worker_t *worker) {
    while (true) {
        task_t *task = worker_next_task(worker);
        if (!task) return;

        worker_handle_task(worker, task);
    }
}
