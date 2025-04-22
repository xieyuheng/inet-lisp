#pragma once

struct net_matcher_t {
    const net_pattern_t *net_pattern;
    hash_t *value_hash;
    node_t **matched_nodes;
    list_t *principal_name_list;
    list_t *matched_principal_name_list;
};

net_matcher_t *net_matcher_new(const net_pattern_t *net_pattern);
void net_matcher_destroy(net_matcher_t **self_pointer);

void net_matcher_print(const net_matcher_t *self, file_t *file);

net_matcher_t *match_net(const net_pattern_t *net_pattern, size_t starting_index, node_t *node);
