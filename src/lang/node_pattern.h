#pragma once

struct node_pattern_t {
    const node_ctor_t *ctor;
    port_info_t **port_infos;
};

node_pattern_t *node_pattern_new(const node_ctor_t *ctor);
void node_pattern_destroy(node_pattern_t **self_pointer);

list_t *node_pattern_make_list(void);

// check principal port agree with ctor.
bool node_pattern_put_port_info(node_pattern_t *self, size_t index, port_info_t *port_info);

bool node_pattern_has_principal_name(node_pattern_t *self, const char *name);

void node_pattern_print(const node_pattern_t *self, file_t *file);
