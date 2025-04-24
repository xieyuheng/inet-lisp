#pragma once

struct node_neighborhood_t {
    node_t *node;
    array_t *node_neighbor_array;
};

node_neighborhood_t *node_neighborhood_new(node_t *node);
void node_neighborhood_destroy(node_neighborhood_t **self_pointer);
