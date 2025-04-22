#pragma once

void compile_set_variable_list(worker_t *worker, function_t *function, list_t *name_list);
void compile_exp_list(worker_t *worker, function_t *function, list_t *exp_list);
void compile_exp(worker_t *worker, function_t *function, exp_t *exp);
