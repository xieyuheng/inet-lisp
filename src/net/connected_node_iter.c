#include "index.h"

struct connected_node_iter_t {
    node_t *root;
    set_t *occurred_node_set;
    list_t *remaining_node_list;
};

connected_node_iter_t *
connected_node_iter_new(node_t *root) {
    assert(root);
    connected_node_iter_t *self = new(connected_node_iter_t);
    self->root = root;
    self->occurred_node_set = set_new();
    self->remaining_node_list = list_new();
    return self;
}

void
connected_node_iter_destroy(connected_node_iter_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    connected_node_iter_t *self = *self_pointer;
    set_destroy(&self->occurred_node_set);
    list_destroy(&self->remaining_node_list);
    free(self);
    *self_pointer = NULL;
}

node_t *
connected_node_iter_first(connected_node_iter_t *self) {
    node_t *node = self->root;
    set_add(self->occurred_node_set, node);

    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (!is_wire(node_get_value(node, i))) continue;

        wire_t *wire = as_wire(node_get_value(node, i));
        if (wire->opposite && is_wire(wire->opposite)) {
            wire_t *opposite_wire = as_wire(wire->opposite);
            if (opposite_wire->node == NULL)
                continue;

            if (set_has(self->occurred_node_set, opposite_wire->node) ||
                list_has(self->remaining_node_list, opposite_wire->node))
                continue;

            list_push(self->remaining_node_list, opposite_wire->node);
        }
    }

    return node;
}

node_t *
connected_node_iter_next(connected_node_iter_t *self) {
    node_t *node = list_pop(self->remaining_node_list);
    if (!node) return NULL;

    set_add(self->occurred_node_set, node);

    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (!is_wire(node_get_value(node, i))) continue;

        wire_t *wire = as_wire(node_get_value(node, i));
        if (wire->opposite && is_wire(wire->opposite)) {
            wire_t *opposite_wire = as_wire(wire->opposite);
            if (opposite_wire->node == NULL ||
                set_has(self->occurred_node_set, opposite_wire->node) ||
                list_has(self->remaining_node_list, opposite_wire->node))
                continue;

            list_push(self->remaining_node_list, opposite_wire->node);
        }
    }

    return node;
}
