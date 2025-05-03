#include "index.h"

extern object_spec_t node_ctor_object_spec;

struct node_ctor_t {
    object_spec_t *spec;
    char *name;
    size_t arity;
    port_info_t **port_infos;
    array_t *rule_array;

    // for primitive node
    void *primitive;
};

node_ctor_t *node_ctor_new(const char *name, size_t arity);
void node_ctor_destroy(node_ctor_t **self_pointer);

inline bool
is_node_ctor(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &node_ctor_object_spec;
}

inline node_ctor_t *
as_node_ctor(value_t value) {
    assert(is_node_ctor(value));
    return (node_ctor_t *) value;
}

size_t node_ctor_find_port_index(const node_ctor_t *node_ctor, const char *port_name);

void node_ctor_print(const node_ctor_t *self, file_t *file);

size_t node_ctor_principal_port_count(const node_ctor_t *self);
