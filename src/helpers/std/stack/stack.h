#pragma once

// stack implemented by growable array

struct stack_t {
    array_t *array;
};

stack_t *stack_new(void);
void stack_purge(stack_t *self);
void stack_destroy(stack_t **self_pointer);

void stack_set_destroy_fn(stack_t *self, destroy_fn_t *destroy_fn);
stack_t *stack_new_with(destroy_fn_t *destroy_fn);

inline size_t
stack_length(const stack_t *self) {
    return array_length(self->array);
}

inline bool
stack_is_empty(const stack_t *self) {
    return array_is_empty(self->array);
}

inline void *
stack_top(stack_t *self) {
    return array_top(self->array);
}

inline void *
stack_pop(stack_t *self) {
    return array_pop(self->array);
}

inline void
stack_push(stack_t *self, void *value) {
    array_push(self->array, value);
}

inline void *
stack_get(const stack_t *self, size_t index) {
    return array_get(self->array, index);
}

inline void *
stack_pick(const stack_t *self, size_t index) {
    return array_pick(self->array, index);
}

inline void
stack_tuck_n(stack_t *self, void *target, size_t n) {
    list_t *value_list = list_new();
    for (size_t i = 0; i < n; i++) {
        void * value = stack_pop(self);
        assert(value);
        list_unshift(value_list, value);
    }

    stack_push(self, target);

    void *value = list_first(value_list);
    while (value) {
        stack_push(self, value);
        value = list_next(value_list);
    }

    list_destroy(&value_list);
}
