#include "index.h"

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

hash_t *
build_node_neighborhood_hash(node_allocator_t *node_allocator) {
    hash_t *node_neighborhood_hash = hash_new();
    hash_set_destroy_fn(node_neighborhood_hash, (destroy_fn_t *) array_destroy);

    array_t *node_array = allocated_node_array(node_allocator);
    size_t length = array_length(node_array);
    for (size_t i = 0; i < length; i++) {
        node_t *x = array_get(node_array, i);
        array_t *node_neighbor_array =
            array_new_auto_with((destroy_fn_t *) node_neighbor_destroy);
        hash_set(node_neighborhood_hash, x, node_neighbor_array);

        for (size_t j = 0; j < length; j++) {
            node_t *y = array_get(node_array, j);
            node_neighbor_t *node_neighbor = node_neighbor_new_maybe(x, y);
            if (node_neighbor) {
                array_push(node_neighbor_array, node_neighbor);
            }
        }
    }

    array_destroy(&node_array);
    return node_neighborhood_hash;
}

void
node_neighbor_array_print(array_t *node_neighbor_array, file_t *file) {
    assert(node_neighbor_array);
    size_t length = array_length(node_neighbor_array);
    for (size_t i = 0; i < length; i++) {
        node_neighbor_t *node_neighbor = array_get(node_neighbor_array, i);        node_neighbor_print(node_neighbor, file);
        fprintf(file, "\n");
    }
}
