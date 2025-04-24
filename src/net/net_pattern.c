#include "index.h"

struct net_pattern_t {
    array_t *node_pattern_array;
    set_t *local_name_set;
};

static void
init_local_name_set(set_t *local_name_set, array_t *node_pattern_array) {
    for (size_t i = 0; i < array_length(node_pattern_array); i++) {
        node_pattern_t *node_pattern = array_get(node_pattern_array, i);
        for (size_t i = 0; i < node_pattern->ctor->arity; i++) {
            port_info_t *port_info = node_pattern->port_infos[i];
            if (!port_info->is_principal) {
                set_put(local_name_set, string_copy(port_info->name));
            }
        }
    }
}

net_pattern_t *
net_pattern_new(list_t *node_pattern_list) {
    net_pattern_t *self = new(net_pattern_t);

    self->node_pattern_array = array_new_auto();
    node_pattern_t *node_pattern = list_first(node_pattern_list);
    while (node_pattern) {
        array_push(self->node_pattern_array, node_pattern);
        node_pattern = list_next(node_pattern_list);
    }

    self->local_name_set = string_set_new();
    init_local_name_set(self->local_name_set, self->node_pattern_array);
    return self;
}

void
net_pattern_destroy(net_pattern_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    net_pattern_t *self = *self_pointer;
    array_destroy(&self->node_pattern_array);
    set_destroy(&self->local_name_set);
    free(self);
    *self_pointer = NULL;
}

size_t
net_pattern_length(const net_pattern_t *self) {
    return array_length(self->node_pattern_array);
}

node_pattern_t *
net_pattern_get(const net_pattern_t *self, size_t index) {
    return array_get(self->node_pattern_array, index);
}

void
net_pattern_print(const net_pattern_t *self, file_t *file) {
    fprintf(file, "<net-pattern>\n");

    fprintf(file, "<node-pattern-list>\n");
    for (size_t i = 0; i < array_length(self->node_pattern_array); i++) {
        node_pattern_t *node_pattern = array_get(self->node_pattern_array, i);
        node_pattern_print(node_pattern, file);
        printf("\n");
    }
    fprintf(file, "</node-pattern-list>\n");

    fprintf(file, "</net-pattern>\n");
}
