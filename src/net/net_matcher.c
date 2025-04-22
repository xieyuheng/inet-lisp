#include "index.h"

net_matcher_t *
net_matcher_new(const net_pattern_t *net_pattern) {
    net_matcher_t *self = new(net_matcher_t);
    self->net_pattern = net_pattern;
    self->value_hash = hash_of_string_key();
    self->matched_nodes = allocate_pointers(net_pattern_length(net_pattern));
    self->principal_name_list = string_list_new();
    self->matched_principal_name_list = string_list_new();
    return self;
}

void
net_matcher_destroy(net_matcher_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    net_matcher_t *self = *self_pointer;
    hash_destroy(&self->value_hash);
    free(self->matched_nodes);
    list_destroy(&self->principal_name_list);
    list_destroy(&self->matched_principal_name_list);
    free(self);
    *self_pointer = NULL;
}

static bool
match_value(value_t first, value_t second) {
    if (is_wire(first) && as_wire(first)->opposite == second)
        return true;

    if (!is_wire(first) && !is_wire(second))
        return first == second;

    return false;
}

static void
matcher_match_node(net_matcher_t *self, size_t index, node_t *node) {
    const node_pattern_t *node_pattern =
        net_pattern_get(self->net_pattern, index);

    if (node_pattern->ctor != node->ctor) return;

    for (size_t i = 0; i < node->ctor->arity; i++) {
        port_info_t *port_info = node_pattern->port_infos[i];
        value_t value = node_get_value(node, i);
        if (value == NULL) return;

        if (port_info->is_principal) {
            if (hash_has(self->value_hash, port_info->name)) {
                value_t found = hash_get(self->value_hash, port_info->name);
                if (!match_value(value, found)) return;
            } else {
                list_push(self->principal_name_list, string_copy(port_info->name));
                assert(hash_set(self->value_hash, string_copy(port_info->name), value));
            }
        } else {
            if (!hash_set(self->value_hash, string_copy(port_info->name), value))
                return;
        }
    }

    self->matched_nodes[index] = node;
}

static const char *
matcher_next_principal_name(net_matcher_t *self) {
    char *name = list_pop(self->principal_name_list);
    if (!name) return NULL;

    list_push(self->matched_principal_name_list, name);
    return name;
}

static bool
matcher_index_is_used(net_matcher_t *self, size_t index) {
    return self->matched_nodes[index] != NULL;
}

static size_t
matcher_next_index(net_matcher_t *self, const char *name) {
    size_t length = net_pattern_length(self->net_pattern);
    for (size_t i = 0; i < length; i++) {
        node_pattern_t *node_pattern = net_pattern_get(self->net_pattern, i);
        if (node_pattern_has_principal_name(node_pattern, name) &&
            !matcher_index_is_used(self, i))
        {
            return i;
        }
    }

    fprintf(stderr, "[matcher_next_index] can not find index for name: %s", name);
    exit(1);
}

static node_t *
matcher_next_node(net_matcher_t *self, const char *name) {
    value_t value = hash_get(self->value_hash, name);
    if (!is_wire(value)) return NULL;

    wire_t *wire = as_wire(value);
    if (!wire) return NULL;
    if (!is_wire(wire->opposite)) return NULL;
    if (!as_wire(wire->opposite)->node) return NULL;

    return as_wire(wire->opposite)->node;
}

static bool
matcher_is_success(const net_matcher_t *self) {
    size_t length = net_pattern_length(self->net_pattern);
    for (size_t i = 0; i < length; i++) {
        if (self->matched_nodes[i] == NULL) return false;
    }

    return true;
}

static void
matcher_start(net_matcher_t *self, size_t starting_index, node_t *node) {
    size_t index = starting_index;
    matcher_match_node(self, index, node);
    const char *name = matcher_next_principal_name(self);
    while (name) {
        node = matcher_next_node(self, name);
        if (node == NULL) return;
        if (node->is_matched) return;

        index = matcher_next_index(self, name);
        matcher_match_node(self, index, node);

        name = matcher_next_principal_name(self);
    }
}

void
net_matcher_print(const net_matcher_t *self, file_t *file) {
    printf("<net-matcher>\n");

    net_pattern_print(self->net_pattern, file);

    printf("<value-hash>\n");
    value_t value = hash_first(self->value_hash);
    while (value) {
        printf("%s:\n", (char *) hash_cursor(self->value_hash));
        printf("  ");
        value_print(value, file);
        printf("\n");
        value = hash_next(self->value_hash);
    }
    printf("</value-hash>\n");

    printf("<matched-nodes>\n");
    size_t length = net_pattern_length(self->net_pattern);
    for (size_t i = 0; i < length; i++) {
        if (self->matched_nodes[i] == NULL) {
            printf("%lu: NULL\n", i);
        } else {
            printf("%lu: ", i);
            node_print(self->matched_nodes[i], file);
            printf("\n");
        }
    }
    printf("</matched-nodes>\n");

    printf("<principal-name-list>");
    string_list_print(self->principal_name_list, ", ", file);
    printf("</principal-name-list>\n");

    printf("<matched-principal-name-list>");
    string_list_print(self->matched_principal_name_list, ", ", file);
    printf("</matched-principal-name-list>\n");

    printf("</net-matcher>\n");
}

net_matcher_t *
match_net(const net_pattern_t *net_pattern, size_t starting_index, node_t *node) {
    net_matcher_t *self = net_matcher_new(net_pattern);
    matcher_start(self, starting_index, node);
    if (!matcher_is_success(self)) {
        // {
        //     printf("[match_net] starting index: %lu\n", starting_index);
        //     printf("[match_net] node: ");
        //     node_print(node, stdout);
        //     printf("\n");
        //     printf("[match_net] failed\n");
        //     net_matcher_print(self, stdout);
        //     printf("\n");
        // }

        net_matcher_destroy(&self);
        return NULL;
    }

    // {
    //     printf("[match_net] starting index: %lu\n", starting_index);
    //     printf("[match_net] node: ");
    //     node_print(node, stdout);
    //     printf("\n");
    //     printf("[match_net] successed\n");
    //     net_matcher_print(self, stdout);
    //     printf("\n");
    // }

    return self;
}
