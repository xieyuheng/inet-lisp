#pragma once

struct task_t {
    const rule_t *rule;
    net_matcher_t *net_matcher;
};

task_t *task_from_rule(const rule_t *rule, net_matcher_t *net_matcher);
void task_destroy(task_t **self_pointer);

void maybe_return_task_by_node(worker_t *worker, node_t *node);
void maybe_return_task_by_node_and_neighbor(worker_t *worker, node_t *node);

void task_print(task_t *self, file_t *file);
