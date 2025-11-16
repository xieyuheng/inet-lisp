#include "index.h"

node_t *
node_new(void) {
    node_t *self = new(node_t);
    self->fast_spinlock = make_fast_spinlock();
    self->value_array = make_array_auto();
    atomic_init(&self->atomic_primitive_arg_count, 0);
    return self;
}

void
node_destroy(node_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    node_t *self = *self_pointer;
    fast_spinlock_destroy(&self->fast_spinlock);
    array_destroy(&self->value_array);
    free(self);
    *self_pointer = NULL;
}

void
node_clean(node_t *self) {
    self->ctor = NULL;
    self->locked_by_worker = NULL;
    self->is_allocated = false;
    array_purge(self->value_array);
    atomic_init(&self->atomic_primitive_arg_count, 0);
}

void
node_put_value(node_t *self, size_t index, value_t value) {
    assert(self);
    array_set(self->value_array, index, value);
}

value_t
node_get_value(const node_t *self, size_t index) {
    assert(self);
    assert(self->ctor);
    return array_get(self->value_array, index);
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

void
node_print_port_info(const node_t *self, size_t index, file_t *file) {
    if (self->ctor) {
        port_info_t *port_info = node_get_port_info(self, index);
        fprintf(file, ":%s", port_info->name);
        if (port_info->is_principal) {
            fprintf(file, "!");
        }
    } else {
        fprintf(file, ":[%lu]", index);
    }
}

bool
node_has_wire(node_t *node, wire_t *wire) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (is_fuzed(node_get_value(node, i), wire))
            return true;
    }

    return false;
}

size_t
node_wire_index(node_t *node, wire_t *wire) {
    for (size_t i = 0; i < node->ctor->arity; i++) {
        if (is_fuzed(node_get_value(node, i), wire))
            return i;
    }

    assert(false);
}

bool
node_is_primitive(const node_t *self) {
    return self->ctor->primitive != NULL;
}

size_t
node_primitive_arg_count_fetch_add1(node_t *self) {
    return atomic_fetch_add_explicit(
        &self->atomic_primitive_arg_count,
        1,
        memory_order_release);
}

void
node_lock(node_t *self) {
    fast_spinlock_lock(self->fast_spinlock);
}

bool
node_try_lock(node_t *self) {
    return fast_spinlock_try_lock(self->fast_spinlock);
}

void
node_unlock(node_t *self) {
    fast_spinlock_unlock(self->fast_spinlock);
}

size_t
node_principal_port_count(const node_t *self) {
    assert(self->ctor);
    return node_ctor_principal_port_count(self->ctor);
}
