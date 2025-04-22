#pragma once

struct task_t {
    const rule_t *rule;
    net_matcher_t *net_matcher;
    node_t *primitive_node;
};

task_t *task_from_rule(const rule_t *rule, net_matcher_t *net_matcher);
task_t *task_from_primitive_node(node_t *primitive_node);
void task_destroy(task_t **self_pointer);

bool task_is_primitive(const task_t *self);

void maybe_return_task_by_node(worker_t *worker, node_t *node);
void maybe_return_task_by_node_and_neighbor(worker_t *worker, node_t *node);

void task_print(task_t *self, file_t *file);
