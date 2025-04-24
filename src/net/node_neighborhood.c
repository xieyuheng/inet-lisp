#include "index.h"

struct node_neighborhood_t {
    node_t *node;
    array_t *node_neighbor_array;
};

node_neighborhood_t *
node_neighborhood_new(node_t *node) {
    node_neighborhood_t *self = new(node_neighborhood_t);
    self->node = node;
    self->node_neighbor_array = array_new_auto_with((destroy_fn_t *) node_neighbor_destroy);
    return self;
}

void
node_neighborhood_destroy(node_neighborhood_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_neighborhood_t *self = *self_pointer;
    array_destroy(&self->node_neighbor_array);
    free(self);
    *self_pointer = NULL;
}

node_t *
node_neighborhood_node(node_neighborhood_t *self) {
    return self->node;
}

node_neighbor_t *
node_neighborhood_get(node_neighborhood_t *self, size_t index) {
    return array_get(self->node_neighbor_array, index);
}

void
node_neighborhood_maybe_add_node(node_neighborhood_t *self, node_t *node) {
    node_neighbor_t *node_neighbor = node_neighbor_new_maybe(self->node, node);
    if (node_neighbor) {
        array_set(
            self->node_neighbor_array,
            node_neighbor->start_port_index,
            node_neighbor);
    }
}

static void
node_neighbor_print_end_port(const node_neighbor_t *self, file_t *file) {
    port_info_t *end_port_info = node_get_port_info(self->end_node, self->end_port_index);

    if (end_port_info->is_principal)
        fprintf(file, "-<>-!%s-", end_port_info->name);
    else
        fprintf(file, "-<>-%s-", end_port_info->name);

    node_print(self->end_node, file);
}

void
node_neighborhood_print(node_neighborhood_t *self, file_t *file) {
    fprintf(file, "(");
    node_print_name(self->node, file);
    fprintf(file, "\n");
    size_t length = self->node->ctor->arity;
    for (size_t i = 0; i < length; i++) {
        port_info_t *port_info = node_get_port_info(self->node, i);
        if (port_info->is_principal)
            fprintf(file, " :%s! ", port_info->name);
        else
            fprintf(file, " :%s ", port_info->name);

        node_neighbor_t *node_neighbor = array_get(self->node_neighbor_array, i);
        if (node_neighbor) {
            node_neighbor_print_end_port(node_neighbor, file);
        } else {
            fprintf(file, "empty");
        }

        if (i < length - 1) {
            fprintf(file, "\n");
        }
    }

    fprintf(file, ")");
    fprintf(file, "\n");
}

hash_t *
build_node_neighborhood_hash(node_allocator_t *node_allocator) {
    hash_t *node_neighborhood_hash = hash_new();
    hash_set_destroy_fn(node_neighborhood_hash, (destroy_fn_t *) node_neighborhood_destroy);

    array_t *node_array = allocated_node_array(node_allocator);
    size_t length = array_length(node_array);
    for (size_t i = 0; i < length; i++) {
        node_t *x = array_get(node_array, i);
        node_neighborhood_t *node_neighborhood = node_neighborhood_new(x);
        hash_set(node_neighborhood_hash, x, node_neighborhood);
        for (size_t j = 0; j < length; j++) {
            node_t *y = array_get(node_array, j);
            node_neighborhood_maybe_add_node(node_neighborhood, y);
        }
    }

    array_destroy(&node_array);
    return node_neighborhood_hash;
}
