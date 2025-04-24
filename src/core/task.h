#pragma once

struct task_t {
    wire_t *left;
    wire_t *right;
    const rule_t *rule;
};

task_t *task_new(wire_t *left, wire_t *right, const rule_t *rule);
void task_destroy(task_t **self_pointer);

void task_print(task_t *self, file_t *file);
