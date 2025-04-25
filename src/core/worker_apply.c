#include "index.h"

void
worker_apply(worker_t *worker, value_t target, size_t arity) {
    if (is_node_ctor(target)) {
        node_t *node = worker_new_node(worker, as_node_ctor(target));
        worker_reconnect_node(worker, node, arity);
    } else if (is_function(target)) {
        // TODO use `arity`
        frame_t *frame = frame_new(as_function(target));
        stack_push(worker->return_stack, frame);
    } else if (is_primitive(target)) {
        // TODO use `arity`
        worker_apply_primitive(worker, as_primitive(target));
    } else {
        who_printf("unknown target: ");
        value_print(target, stdout);
        printf("\n");
        exit(1);
    }
}
