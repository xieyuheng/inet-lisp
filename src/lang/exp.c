#include "index.h"

exp_t *
exp_var(char *name) {
    exp_t *self = new(exp_t);
    self->kind = EXP_VAR;
    self->var.name = name;
    return self;
}

exp_t *
exp_ap(exp_t *target, list_t *arg_list) {
    exp_t *self = new(exp_t);
    self->kind = EXP_AP;
    self->ap.target = target;
    self->ap.arg_list = arg_list;
    return self;
}

exp_t *
exp_assign(list_t *name_list, exp_t *exp) {
    exp_t *self = new(exp_t);
    self->kind = EXP_ASSIGN;
    self->assign.name_list = name_list;
    self->assign.exp = exp;
    return self;
}

exp_t *
exp_int(int64_t target) {
    exp_t *self = new(exp_t);
    self->kind = EXP_INT;
    self->i.target = target;
    return self;
}

exp_t *
exp_float(double target) {
    exp_t *self = new(exp_t);
    self->kind = EXP_FLOAT;
    self->f.target = target;
    return self;
}

list_t *
exp_make_list(void) {
    return make_list_with((destroy_fn_t *) exp_destroy);
}

void
exp_destroy(exp_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    exp_t *self = *self_pointer;
    switch (self->kind) {
    case EXP_VAR: {
        string_destroy(&self->var.name);
        break;
    }

    case EXP_AP: {
        exp_destroy(&self->ap.target);
        list_destroy(&self->ap.arg_list);
        break;
    }

    case EXP_ASSIGN: {
        list_destroy(&self->assign.name_list);
        exp_destroy(&self->assign.exp);
        break;
    }

    case EXP_INT: {
        break;
    }

    case EXP_FLOAT: {
        break;
    }
    }

    free(self);
    *self_pointer = NULL;
    return;
}

exp_t *
exp_copy(const exp_t *self) {
    switch (self->kind) {
    case EXP_VAR: {
        return exp_var(string_copy(self->var.name));
    }

    case EXP_AP: {
        return exp_ap(
            exp_copy(self->ap.target),
            exp_list_copy(self->ap.arg_list));
    }

    case EXP_ASSIGN: {
        return exp_assign(
            string_list_copy(self->assign.name_list),
            exp_copy(self->assign.exp));
    }

    case EXP_INT: {
        return exp_int(self->i.target);
    }

    case EXP_FLOAT: {
        return exp_float(self->f.target);
    }
    }

    assert(false);
}

list_t *exp_list_copy(list_t *exp_list) {
    list_set_copy_fn(exp_list, (copy_fn_t *) exp_copy);
    return list_copy(exp_list);
}

void
exp_list_print(list_t *exp_list, file_t *file) {
    exp_t *exp = list_first(exp_list);
    while (exp) {
        exp_print(exp, file);
        if (!list_cursor_is_end(exp_list))
            fprintf(file, " ");
        exp = list_next(exp_list);
    }
}

void
exp_list_print_as_tail(list_t *exp_list, file_t *file) {
    if (list_is_empty(exp_list)) {
        fprintf(file, ")");
    } else {
        fprintf(file, " ");
        exp_list_print(exp_list, file);
        fprintf(file, ")");
    }
}

void
exp_list_print_as_list(list_t *exp_list, file_t *file) {
    fprintf(file, "(");
    exp_list_print(exp_list, file);
    fprintf(file, ")");
}

void
name_list_print(list_t *name_list, file_t *file) {
    char *name = list_first(name_list);
    while (name) {
        if (list_cursor_is_end(name_list))
            fprintf(file, "%s", name);
        else
            fprintf(file, "%s ", name);
        name = list_next(name_list);
    }
}

void
exp_print(const exp_t *self, file_t *file) {
    switch (self->kind) {
    case EXP_VAR: {
        fprintf(file, "%s", self->var.name);
        return;
    }

    case EXP_AP: {
        if (list_is_empty(self->ap.arg_list)) {
            fprintf(file, "(");
            exp_print(self->ap.target, file);
            fprintf(file, ")");
            return;
        }

        fprintf(file, "(");
        exp_print(self->ap.target, file);
        fprintf(file, " ");
        exp_list_print(self->ap.arg_list, file);
        fprintf(file, ")");
        return;
    }

    case EXP_ASSIGN: {
        if (list_is_empty(self->assign.name_list)) {
            fprintf(file, "(=)");
            return;
        }

        fprintf(file, "(= ");
        name_list_print(self->assign.name_list, file);
        fprintf(file, ")");
        return;
    }

    case EXP_INT: {
        value_print(xint(self->i.target), file);
        return;
    }

    case EXP_FLOAT: {
        value_print(xfloat(self->f.target), file);
        return;
    }
    }
}
