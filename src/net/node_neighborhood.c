#include "index.h"

node_neighborhood_t *
node_neighborhood_new(node_t *node) {
    node_neighborhood_t *self = new(node_neighborhood_t);
    self->node = node;
    self->node_neighbor_array = array_new_auto();
    return self;
}
