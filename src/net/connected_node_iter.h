#pragma once

connected_node_iter_t *connected_node_iter_new(node_t *root);
void connected_node_iter_destroy(connected_node_iter_t **self_pointer);

node_t *connected_node_iter_first(connected_node_iter_t *self);
node_t *connected_node_iter_next(connected_node_iter_t *self);
