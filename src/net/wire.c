#include "index.h"

object_spec_t wire_object_spec = {
    .name = "wire",
    .print_fn = (print_fn_t *) wire_print,
};

wire_t *
wire_new(void) {
    wire_t *self = new(wire_t);
    self->spec = &wire_object_spec;
    self->node = NULL;
    self->opposite = NULL;
    return self;
}

void
wire_destroy(wire_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    wire_t *self = *self_pointer;
    free(self);
    // Does NOT own `node` and `opposite`.
    *self_pointer = NULL;
}

bool
is_wire(value_t value) {
    if (!is_xobject(value)) return false;
    return as_object(value)->spec == &wire_object_spec;
}

wire_t *
as_wire(value_t value) {
    assert(is_wire(value));
    return (wire_t *) value;
}

const char *
wire_name(const wire_t *self) {
    assert(self->node);
    port_info_t *port_info = self->node->ctor->port_infos[self->index];
    assert(port_info);
    return port_info->name;
}

const char *
wire_node_name(const wire_t *self) {
    assert(self->node);
    return self->node->ctor->name;
}

void
wire_free_from_node(wire_t *self) {
    self->node = NULL;
}

bool
wire_is_free(const wire_t *self) {
    if (self->node) return false;

    return true;
}

bool
wire_is_principal(const wire_t *self) {
    if (!self->node) return false;
    if (!self->node->ctor) return false;

    port_info_t *port_info = self->node->ctor->port_infos[self->index];
    return port_info->is_principal;
}

static void
wire_print_half_at_left(const wire_t *self, file_t *file) {
    if (!self->node) {
        fprintf(file, "-<");
        return;
    }

    node_print(self->node, file);

    if (wire_is_principal(self)) {
        fprintf(file, "-%s!-<", wire_name(self));
    } else {
        fprintf(file, "-%s-<", wire_name(self));
    }
}

static void
wire_print_half_at_right(const wire_t *self, file_t *file) {
    if (!self->node) {
        fprintf(file, ">-");
        return;
    }

    if (wire_is_principal(self)) {
        fprintf(file, ">-!%s-", wire_name(self));
    } else {
        fprintf(file, ">-%s-", wire_name(self));
    }

    node_print(self->node, file);
}

void
wire_print(const wire_t *self, file_t *file) {
    wire_print_half_at_left(self, file);

    if (self->opposite) {
        if (is_wire(self->opposite)) {
            wire_print_half_at_right(self->opposite, file);
        } else {
            fprintf(file, "[");
            value_print(self->opposite, file);
            fprintf(file, "]");
        }
    }
}

static void
wire_print_within_node(const wire_t *self, file_t *file) {
    fprintf(file, "-<");

    if (self->opposite) {
        if (is_wire(self->opposite)) {
            wire_print_half_at_right(self->opposite, file);
        } else {
            fprintf(file, "[");
            value_print(self->opposite, file);
            fprintf(file, "]");
        }
    }
}

void
wire_print_net(wire_t *self, file_t *file) {
    fprintf(file, "<net>\n");

    fprintf(file, ":root ");
    value_print(self, file);
    fprintf(file, "\n");

    node_t *root_node = self->node;
    if (!root_node) {
        if (is_wire(self->opposite)) {
            root_node = as_wire(self->opposite)->node;
        }
    }

    if (root_node) {
        connected_node_iter_t *iter = connected_node_iter_new(root_node);
        node_t *node = connected_node_iter_first(iter);
        while (node) {
            fprintf(file, "(");
            node_print_name(node, file);
            fprintf(file, "\n");
            for (size_t i = 0; i < node->ctor->arity; i++) {
                port_info_t *port_info = node->ctor->port_infos[i];
                if (port_info->is_principal)
                    fprintf(file, " :%s! ", port_info->name);
                else
                    fprintf(file, " :%s ", port_info->name);

                value_t value = node_get_value(node, i);
                if (value) {
                    if (is_wire(value)) {
                        wire_print_within_node(as_wire(value), file);
                    } else {
                        value_print(value, file);
                    }
                }

                if (i < node->ctor->arity - 1)
                    fprintf(file, "\n");
            }
            fprintf(file, ")");
            fprintf(file, "\n");
            node = connected_node_iter_next(iter);
        }

        connected_node_iter_destroy(&iter);
    }

    fprintf(file, "</net>\n");
}
