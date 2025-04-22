#include "index.h"

void
worker_work(worker_t *worker) {
    while (true) {
        task_t *task = list_pop(worker->task_list);
        if (!task) return;
        worker_handle_task(worker, task);
    }
}
