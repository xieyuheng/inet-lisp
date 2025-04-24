#include "index.h"

node_neighborhood_t *
node_neighborhood_new(node_t *node) {
    node_neighborhood_t *self = new(node_neighborhood_t);
    self->node = node;
    self->node_neighbor_array = array_new_auto();
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
