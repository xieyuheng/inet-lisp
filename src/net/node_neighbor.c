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

void
node_neighbor_print(const node_neighbor_t *self, file_t *file) {
    port_info_t *start_port_info = node_get_port_info(self->start_node, self->start_port_index);
    port_info_t *end_port_info = node_get_port_info(self->end_node, self->end_port_index);

    if (start_port_info->is_principal)
        fprintf(file, " :%s! ", start_port_info->name);
    else
        fprintf(file, " :%s ", start_port_info->name);

    if (end_port_info->is_principal)
        fprintf(file, "-<>-!%s-", end_port_info->name);
    else
        fprintf(file, "-<>-%s-", end_port_info->name);

    node_print(self->end_node, file);
}
