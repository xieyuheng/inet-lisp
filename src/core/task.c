#include "index.h"

task_t *
task_new(principal_wire_t *left, principal_wire_t *right, const rule_t *rule) {
    task_t *self = new(task_t);
    self->left = left;
    self->right = right;
    self->rule = rule;
#if DEBUG_TASK_LOCK
    self->mutex = mutex_new();
#endif
    return self;
}

task_t *
task_new_primitve(node_t *primitive_node) {
    task_t *self = new(task_t);
    self->primitive_node = primitive_node;
#if DEBUG_TASK_LOCK
    self->mutex = mutex_new();
#endif
    return self;
}

void
task_destroy(task_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    task_t *self = *self_pointer;
    free(self);
    *self_pointer = NULL;
}

bool
task_is_primitive(const task_t *self) {
    return self->primitive_node != NULL;
}

void
task_print(const task_t *self, file_t *file) {
    principal_wire_print_left(self->left, file);
    principal_wire_print_right(self->right, file);
}
