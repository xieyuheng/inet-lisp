#include "index.h"

node_t *
node_new(void) {
    node_t *self = new(node_t);
    self->mutex = mutex_new();
    self->values = allocate_pointers(NODE_MAX_ARITY);
    atomic_init(&self->atomic_primitive_arg_count, 0);
    return self;
}

void
node_destroy(node_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_t *self = *self_pointer;
    mutex_destroy(&self->mutex);
    free(self->values);
    free(self);
    *self_pointer = NULL;
}

void
node_clean(node_t *self) {
    self->ctor = NULL;
    self->locked_by_worker = NULL;
    self->is_allocated = false;
    memset(self->values, 0, NODE_MAX_ARITY * sizeof(void *));
}

void
node_set_value(node_t *self, size_t index, value_t value) {
    assert(self);
    assert(index < NODE_MAX_ARITY);
    self->values[index] = value;
}

value_t
node_get_value(const node_t *self, size_t index) {
    assert(self);
    assert(self->ctor);
    assert(index < NODE_MAX_ARITY);
    return self->values[index];
}

port_info_t *
node_get_port_info(const node_t *self, size_t index) {
    assert(self->ctor);
    assert(index < self->ctor->arity);
    return self->ctor->port_infos[index];
}

void
node_print_name(const node_t *self, file_t *file) {
    if (self->ctor) {
        char *id_string = uint_to_subscript(self->id);
        fprintf(file, "%s%s", self->ctor->name, id_string);
        free(id_string);
    } else {
        char *id_string = uint_to_subscript(self->id);
        fprintf(file, "%s", id_string);
        free(id_string);
    }
}

void
node_print(const node_t *self, file_t *file) {
    fprintf(file, "(");
    node_print_name(self, file);
    fprintf(file, ")");
}

bool
node_has_wire(node_t *node, wire_t *wire) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (is_fuzed(node_get_value(node, i), wire))
            return true;
    }

    return false;
}

bool
node_is_primitive(const node_t *self) {
    return self->ctor->primitive != NULL;
}

size_t
node_primitive_arg_count_fetch_add1(node_t *self) {
    atomic_fetch_add_explicit(
        &self->atomic_primitive_arg_count,
        1,
        memory_order_release);
}
