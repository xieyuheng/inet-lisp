#include "index.h"

void
worker_apply(worker_t *worker, value_t target, size_t arity) {
    if (is_node_ctor(target)) {
        node_t *node = worker_new_node(worker, as_node_ctor(target));
        worker_reconnect_node(worker, node, arity);
    } else if (is_function(target)) {
        function_t *function = as_function(target);
        // TODO supplement wire if arity is short
        assert(function->arity == arity);
        frame_t *frame = frame_new(function);
        stack_push(worker->return_stack, frame);
    } else if (is_primitive(target)) {
        primitive_t *primitive = as_primitive(target);
        if (primitive->node_ctor) {
            node_t *node = worker_new_node(worker, primitive->node_ctor);
            worker_reconnect_node(worker, node, arity);
        } else {
            // TODO supplement wire if arity is short
            assert(primitive->input_arity == arity);
            worker_apply_primitive(worker, primitive);
        }
    } else if (is_wire(target)) {
        value_t defuzed = defuze(target);
        if (is_wire(defuzed)) {
            who_printf("can not apply unfuzed wire\n");
            exit(1);
        } else {
            worker_apply(worker, defuzed, arity);
        }
    } else {
        who_printf("unknown target: ");
        value_print(target, stdout);
        printf("\n");
        exit(1);
    }
}
