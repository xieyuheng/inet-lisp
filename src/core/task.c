#include "index.h"

static task_t *
task_new(void) {
    task_t *self = new(task_t);
#if DEBUG_TASK_LOCK
    self->mutex = make_mutex();
#endif
    return self;
}

task_t *
task_active_pair(principal_wire_t *left, principal_wire_t *right, const rule_t *rule) {
    task_t *self = task_new();
    self->kind = TASK_ACTIVE_PAIR;
    self->active_pair.left = left;
    self->active_pair.right = right;
    self->active_pair.rule = rule;
    return self;
}

task_t *
task_primitive(node_t *node) {
    task_t *self = new(task_t);
    self->kind = TASK_PRIMITIVE;
    self->primitive.node = node;
    return self;
}

void
task_destroy(task_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    task_t *self = *self_pointer;
#if DEBUG_TASK_LOCK
    mutex_destroy(&self->mutex);
#endif
    free(self);
    *self_pointer = NULL;
}

void
task_print(const task_t *self, file_t *file) {
    switch (self->kind) {
    case TASK_ACTIVE_PAIR: {
        fprintf(file, "(task-active-pair ");
        principal_wire_print_left(self->active_pair.left, file);
        fprintf(file, " ");
        principal_wire_print_right(self->active_pair.right, file);
        fprintf(file, ")");
        return;
    }

    case TASK_PRIMITIVE: {
        fprintf(file, "(task-primitive ");
        node_t *node = self->primitive.node;
        fprintf(file, "(");
        node_print_name(node, file);
        fprintf(file, " ");
        for (size_t i = 0; i < node->ctor->arity; i++) {
            port_info_t *port_info = node_get_port_info(node, i);
            if (port_info->is_principal) {
                node_print_port_info(node, i, file);
                fprintf(file, " ");
                value_print(node_get_value(node, i), file);
            }
        }

        fprintf(file, ")");
        fprintf(file, ")");
        return;
    }
    }
}
