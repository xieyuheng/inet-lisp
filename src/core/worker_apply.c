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
        primitive_t *primitive = as_primitive(target);
        // TODO supplement wire if arity is short
        assert(primitive->input_arity == arity);
        if (!primitive->node_ctor) {
            worker_apply_primitive(worker, primitive);
        } else {
            node_t *node = worker_new_node(worker, primitive->node_ctor);
            worker_reconnect_node(worker, node, arity);
        }
    } else {
        who_printf("unknown target: ");
        value_print(target, stdout);
        printf("\n");
        exit(1);
    }
}
