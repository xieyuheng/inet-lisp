#pragma once

struct task_t {
    // normal task
    principal_wire_t *left;
    principal_wire_t *right;
    const rule_t *rule;

    // primitive task
    node_t *primitive_node;

#if DEBUG_TASK_LOCK
    mutex_t *mutex;
#endif
};

task_t *task_new(principal_wire_t *left, principal_wire_t *right, const rule_t *rule);
task_t *task_new_primitve(node_t *primitive_node);
void task_destroy(task_t **self_pointer);

bool task_is_primitive(const task_t *self);

void task_print(const task_t *self, file_t *file);
