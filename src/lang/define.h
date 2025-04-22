#pragma once

void define(mod_t *self, const char *name, value_t value);

void define_rule(mod_t *self, const char *name, rule_t *rule);
void define_rule_star(worker_t *worker, list_t *node_pattern_list, list_t *exp_list);

void define_node(worker_t *worker, const char *name, list_t *port_name_list);

void define_primitive_fn(mod_t *mod, const char *name, size_t input_arity, size_t output_arity, primitive_fn_t *primitive_fn);
void define_primitive_fn_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0);
void define_primitive_fn_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1);
void define_primitive_fn_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2);
void define_primitive_fn_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3);
void define_primitive_fn_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4);

void define_primitive_node(mod_t *mod, const char *name, const char *port_names[]);

void define_primitive_fn_node(mod_t *mod, const char *name, size_t input_arity, size_t output_arity, primitive_fn_t *primitive_fn, const char *port_names[]);
void define_primitive_fn_node_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0, const char *port_names[]);
void define_primitive_fn_node_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1, const char *port_names[]);
void define_primitive_fn_node_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2, const char *port_names[]);
void define_primitive_fn_node_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3, const char *port_names[]);
void define_primitive_fn_node_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4, const char *port_names[]);
