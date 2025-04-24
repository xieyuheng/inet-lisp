#include "index.h"

node_neighbor_t *
node_neighbor_new_maybe(node_t *start_node, node_t *end_node) {
    if (start_node == end_node)
        return NULL;

    for (size_t i = 0; i < start_node->ctor->arity; i++) {
        for (size_t j = 0; j < end_node->ctor->arity; j++) {
            value_t x = node_get_value(start_node, i);
            value_t y = node_get_value(end_node, j);
            if (is_connected(x, y)) {
                node_neighbor_t *self = new(node_neighbor_t);
                self->start_node = start_node;
                self->start_port_index = i;
                self->end_port_index = j;
                self->end_node = end_node;
                return self;
            }
        }
    }

    return NULL;
}

void
node_neighbor_destroy(node_neighbor_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_neighbor_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}
