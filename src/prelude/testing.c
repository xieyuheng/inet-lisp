#include "index.h"

void x_assert(worker_t *worker) {
    bool ok = to_bool(stack_pop(worker->value_stack));
    if (!ok) {
        fprintf(stderr, "[assert] fail\n");
        worker_print(worker, stderr);
        exit(1);
    }
}
