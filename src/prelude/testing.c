#include "index.h"

void x_assert(worker_t *worker) {
    bool ok = to_bool(stack_pop(worker->value_stack));
    if (!ok) {
        who_printf("assertion fail\n");
        worker_print(worker, stdout);
        exit(1);
    }
}
