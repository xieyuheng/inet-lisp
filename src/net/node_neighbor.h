#pragma once

struct node_neighbor_t {
    node_t *start_node;
    size_t start_port_index;
    size_t end_port_index;
    node_t *end_node;
};

node_neighbor_t *node_neighbor_new_maybe(node_t *start_node, node_t *end_node);
void node_neighbor_destroy(node_neighbor_t **self_pointer);

hash_t *build_node_neighborhood_hash(node_allocator_t *node_allocator);
void node_neighbor_print(const node_neighbor_t *self, file_t *file);
void node_neighbor_print_as_neighbor(const node_neighbor_t *self, file_t *file);

void node_neighbor_array_print(array_t *node_neighbor_array, file_t *file);
