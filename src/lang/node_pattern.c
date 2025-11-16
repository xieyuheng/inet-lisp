#include "index.h"

node_pattern_t *
node_pattern_new(const node_ctor_t *ctor) {
    node_pattern_t *self = new(node_pattern_t);
    self->ctor = ctor;
    self->port_infos = allocate_pointers(ctor->arity);
    return self;
}

void
node_pattern_destroy(node_pattern_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_pattern_t *self = *self_pointer;
    for (size_t i = 0; i < self->ctor->arity; i++) {
        port_info_t *port_info = self->port_infos[i];
        if (port_info) {
            port_info_destroy(&port_info);
        }
    }

    free(self);
    *self_pointer = NULL;
}

list_t *
node_pattern_make_list(void) {
    return make_list_with((destroy_fn_t *) node_pattern_destroy);
}

bool
node_pattern_set_port_info(node_pattern_t *self, size_t index, port_info_t *port_info) {
    if (port_info->is_principal != self->ctor->port_infos[index]->is_principal)
        return false;

    self->port_infos[index] = port_info;
    return true;
}

bool
node_pattern_has_principal_name(node_pattern_t *self, const char *name) {
    for (size_t i = 0; i < self->ctor->arity; i++) {
        if (self->port_infos[i]->is_principal &&
            string_equal(self->port_infos[i]->name, name))
        {
            return true;
        };
    }

    return false;
}

void
node_pattern_print(const node_pattern_t *self, file_t *file) {
    fprintf(file, "(");
    fprintf(file, "%s", self->ctor->name);
    for (size_t i = 0; i < self->ctor->arity; i++) {
        port_info_t *port_info = self->port_infos[i];
        if (port_info->is_principal) {
            fprintf(file, " %s!", port_info->name);
        } else {
            fprintf(file, " %s", port_info->name);
        }
    }

    fprintf(file, ")");
}
