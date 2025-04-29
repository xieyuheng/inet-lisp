#include "index.h"

void
worker_connect(worker_t *worker, value_t left, value_t right) {
    task_t *task = connect(left, right);
    if (task) {
        worker_add_task(worker, task);
    }
}
