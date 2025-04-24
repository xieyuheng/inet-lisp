#include "index.h"

void
worker_disconnect_node(worker_t *worker, node_t *node) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        port_info_t *port_info = node->ctor->port_infos[i];
        value_t value = node_get_value(node, i);
        if (port_info->is_principal) {
            if (is_wire(value)) {
                wire_t *wire = as_wire(value);
                wire_destroy(&wire);
            }
        } else {
            stack_push(worker->value_stack, value);
        }
    }
}
