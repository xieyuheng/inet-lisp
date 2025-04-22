#pragma once

struct rule_t {
    size_t starting_index;
    net_pattern_t *net_pattern;
    function_t *function;
};

rule_t *rule_new(size_t starting_index, net_pattern_t *net_pattern, function_t *function);
void rule_destroy(rule_t **self_pointer);

void rule_print(const rule_t *self, file_t *file);
