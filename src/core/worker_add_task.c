#include "index.h"

void
worker_add_task(worker_t* self, task_t *task) {
    list_unshift(self->task_list, task);
}
