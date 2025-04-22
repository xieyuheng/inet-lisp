#include "index.h"

stmt_t *
stmt_define(char *name, exp_t *exp) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_DEFINE;
    self->define.name = name;
    self->define.exp = exp;
    return self;
}

stmt_t *
stmt_define_function(char *name, list_t *arg_name_list, list_t *exp_list) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_DEFINE_FUNCTION;
    self->define_function.name = name;
    self->define_function.arg_name_list = arg_name_list;
    self->define_function.exp_list = exp_list;
    return self;
}

stmt_t *
stmt_define_node(char *name, list_t *port_name_list) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_DEFINE_NODE;
    self->define_node.name = name;
    self->define_node.port_name_list = port_name_list;
    return self;
}

stmt_t *
stmt_define_rule(exp_t *pattern_exp, list_t *exp_list) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_DEFINE_RULE;
    self->define_rule.pattern_exp = pattern_exp;
    self->define_rule.exp_list = exp_list;
    return self;
}

stmt_t *
stmt_define_rule_star(list_t *pattern_exp_list, list_t *exp_list) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_DEFINE_RULE_STAR;
    self->define_rule_star.pattern_exp_list = pattern_exp_list;
    self->define_rule_star.exp_list = exp_list;
    return self;
}

stmt_t *
stmt_compute_exp(exp_t *exp) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_COMPUTE_EXP;
    self->compute_exp.exp = exp;
    return self;
}

stmt_t *
stmt_import(list_t *name_list, path_t *path) {
    stmt_t *self = new(stmt_t);
    self->kind = STMT_IMPORT;
    self->import.name_list = name_list;
    self->import.path = path;
    return self;
}

void
stmt_destroy(stmt_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    stmt_t *self = *self_pointer;
    switch (self->kind) {
    case STMT_DEFINE: {
        string_destroy(&self->define.name);
        exp_destroy(&self->define.exp);
        break;
    }

    case STMT_DEFINE_FUNCTION: {
        string_destroy(&self->define_function.name);
        list_destroy(&self->define_function.arg_name_list);
        list_destroy(&self->define_function.exp_list);
        break;
    }

    case STMT_DEFINE_NODE: {
        string_destroy(&self->define_node.name);
        list_destroy(&self->define_node.port_name_list);
        break;
    }

    case STMT_DEFINE_RULE: {
        exp_destroy(&self->define_rule.pattern_exp);
        list_destroy(&self->define_rule.exp_list);
        break;
    }

    case STMT_DEFINE_RULE_STAR: {
        list_destroy(&self->define_rule_star.pattern_exp_list);
        list_destroy(&self->define_rule_star.exp_list);
        break;
    }

    case STMT_COMPUTE_EXP: {
        exp_destroy(&self->compute_exp.exp);
        break;
    }

    case STMT_IMPORT: {
        list_destroy(&self->import.name_list);
        path_destroy(&self->import.path);
        break;
    }
    }

    free(self);
    *self_pointer = NULL;
    return;
}

void
stmt_print(const stmt_t *self, file_t *file) {
    switch (self->kind) {
    case STMT_DEFINE: {
        fprintf(file, "(define %s ", self->define_function.name);
        exp_print(self->define.exp, file);
        fprintf(file, ")");
        return;
    }

    case STMT_DEFINE_FUNCTION: {
        fprintf(file, "(define (");
        fprintf(file, "%s", self->define_function.name);
        if (list_is_empty(self->define_function.arg_name_list)) {
            fprintf(file, ")");
        } else {
            fprintf(file, " ");
            name_list_print(self->define_function.arg_name_list, file);
            fprintf(file, ")");
        }

        exp_list_print_as_tail(self->define_function.exp_list, file);
        return;
    }

    case STMT_DEFINE_NODE: {
        fprintf(file, "(define-node %s ", self->define_node.name);
        name_list_print(self->define_node.port_name_list, file);
        fprintf(file, ")");
        return;
    }

    case STMT_DEFINE_RULE: {
        fprintf(file, "(define-rule ");
        exp_print(self->define_rule.pattern_exp, file);
        exp_list_print_as_tail(self->define_rule.exp_list, file);
        return;
    }

    case STMT_DEFINE_RULE_STAR: {
        fprintf(file, "(define-rule* ");
        exp_list_print_as_list(self->define_rule_star.pattern_exp_list, file);
        exp_list_print_as_tail(self->define_rule_star.exp_list, file);
        return;
    }

    case STMT_COMPUTE_EXP: {
        exp_print(self->compute_exp.exp, file);
        return;
    }

    case STMT_IMPORT: {
        fprintf(file, "(import ");
        name_list_print(self->import.name_list, file);
        fprintf(file, " \"%s\"", path_string(self->import.path));
        fprintf(file, ")");
        return;
    }
    }
}
