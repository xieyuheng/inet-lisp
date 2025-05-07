#include "index.h"

task_t *
worker_offer_task(worker_t *worker) {
    return deque_pop_front(worker->task_deque);
}
