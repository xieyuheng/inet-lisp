#include "index.h"

#define STACK_BLOCK_SIZE 64

stack_t *
make_stack(void) {
    stack_t *self = new(stack_t);
    self->array = make_array_auto();
    return self;
}

void
stack_purge(stack_t *self) {
    array_purge(self->array);
}

void
stack_destroy(stack_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    stack_t *self = *self_pointer;
    stack_purge(self);
    array_destroy(&self->array);
    free(self);
    *self_pointer = NULL;
}

void
stack_set_destroy_fn(stack_t *self, destroy_fn_t *destroy_fn) {
    array_set_destroy_fn(self->array, destroy_fn);
}

stack_t *
make_stack_with(destroy_fn_t *destroy_fn) {
    stack_t *self = make_stack();
    stack_set_destroy_fn(self, destroy_fn);
    return self;
}

size_t stack_length(const stack_t *self);
bool stack_is_empty(const stack_t *self);

void *stack_top(stack_t *self);
void *stack_pop(stack_t *self);
void stack_push(stack_t *self, void *value);

void *stack_get(const stack_t *self, size_t index);
void *stack_pick(const stack_t *self, size_t index);

void stack_tuck_n(stack_t *self, void *target, size_t n);
