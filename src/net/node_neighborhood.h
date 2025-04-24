#pragma once

struct node_neighborhood_t {
    node_t *node;
    array_t *node_neighbor_array;
};

node_neighborhood_t *node_neighborhood_new(node_t *node);
void node_neighborhood_destroy(node_neighborhood_t **self_pointer);

hash_t *build_node_neighborhood_hash(node_allocator_t *node_allocator);

void node_neighborhood_print(node_neighborhood_t *self, file_t *file);
