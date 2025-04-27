#pragma once

typedef enum {
    TASK_ACTIVE_PAIR,
    TASK_PRIMITIVE,
} task_kind_t;

struct task_t {
    task_kind_t kind;
#if DEBUG_TASK_LOCK
    mutex_t *mutex;
#endif
    union {
        struct { principal_wire_t *left; principal_wire_t *right; const rule_t *rule; } active_pair;
        struct { node_t *node; } primitive;
    };
};

task_t *task_active_pair(principal_wire_t *left, principal_wire_t *right, const rule_t *rule);
task_t *task_primitive(node_t *node);
void task_destroy(task_t **self_pointer);

bool task_is_primitive(const task_t *self);

void task_print(const task_t *self, file_t *file);
