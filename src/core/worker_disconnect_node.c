#include "index.h"

void
worker_disconnect_node(worker_t *worker, node_t *node) {
#if DEBUG_NODE_LOCK
    bool contention_occurred = false;
    uint64_t start_nanosecond = time_nanosecond();
    while (!node_try_lock(node)) {
        contention_occurred = true;
        file_lock(stdout);
        who_printf("lock contention! ");
        printf("worker id: #%lu, ", worker->id);
        printf("locked by: #%lu, ", ((worker_t *) node->locked_by_worker)->id);
        printf("node: "); node_print(node, stdout);
        printf("\n");
        file_unlock(stdout);
    }

    if (contention_occurred) {
        uint64_t passed_nanosecond = time_passed_nanosecond(start_nanosecond);
        file_lock(stdout);
        who_printf("lock contention time: %lu ns\n", passed_nanosecond);
        file_unlock(stdout);
    }

#else
    node_lock(node);
#endif

    for (size_t i = 0; i < node->ctor->arity; i++) {
        value_t value = node_get_value(node, i);
        if (is_principal_wire(value)) {
            principal_wire_t *principal_wire = as_principal_wire(value);
            principal_wire_destroy(&principal_wire);
        } else {
            stack_push(worker->value_stack, value);
        }
    }

    node_unlock(node);
    worker_recycle_node(worker, node);
}
