#pragma once

node_neighborhood_t *node_neighborhood_new(node_t *node);
void node_neighborhood_destroy(node_neighborhood_t **self_pointer);

node_t *node_neighborhood_node(node_neighborhood_t *self);
node_neighbor_t *node_neighborhood_get(node_neighborhood_t *self, size_t index);
void node_neighborhood_maybe_add_node(node_neighborhood_t *self, node_t *node);

void node_neighborhood_print(node_neighborhood_t *self, const char *prefix, file_t *file);

hash_t *build_node_neighborhood_hash(node_allocator_t *node_allocator);
