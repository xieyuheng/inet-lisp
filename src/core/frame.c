#include "index.h"

#define VARIABLE_ARRAY_SIZE 64

struct frame_t {
    size_t cursor;
    const function_t *function;
    array_t *variable_array;
};

frame_t *
frame_new(const function_t *function) {
    frame_t *self = new(frame_t);
    self->cursor = 0;
    self->function = function;
    self->variable_array = array_new(VARIABLE_ARRAY_SIZE);
    return self;
}

void
frame_destroy(frame_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    frame_t *self = *self_pointer;
    // does not own function
    array_destroy(&self->variable_array);
    free(self);
    *self_pointer = NULL;
}

bool
frame_is_finished(const frame_t *self) {
    return self->cursor == function_length(self->function);
}

opcode_t *
frame_fetch_opcode(frame_t *self) {
    opcode_t *opcode = function_get_opcode(self->function, self->cursor);
    self->cursor++;
    return opcode;
}

void
frame_print(const frame_t *self, file_t *file) {
    fprintf(file, "<frame cursor=\"%lu\">\n", self->cursor);
    function_print_with_cursor(self->function, file, self->cursor);

    size_t size = array_size(self->variable_array);
    fprintf(file, "<variable-array>\n");
    for (size_t i = 0; i < size; i++) {
        value_t value = array_get(self->variable_array, i);
        if (value != NULL) {
            fprintf(file, "%lu: ", i);

            // NOTE since we are calling `value_print` here,
            // we must use linear variable,
            // otherwise a wire might be deleted
            // but still referenced in the `variable_array`.
            value_print(value, file);

            fprintf(file, "\n");
        }
    }
    fprintf(file, "</variable-array>\n");
    fprintf(file, "</frame>\n");
}

value_t
frame_get_variable(const frame_t *self, size_t index) {
    value_t value = array_get(self->variable_array, index);
    if (!value) {
        fprintf(stderr, "[frame_get_variable] undefined variable index: %lu\n", index);
        exit(1);
    }

    // NOTE be linear like this:
    array_set(self->variable_array, index, NULL);
    return value;
}

void
frame_set_variable(frame_t *self, size_t index, value_t value) {
    value_t found = array_get(self->variable_array, index);
    // NOTE be linear like this:
    if (found) {
        fprintf(stderr, "[frame_set_variable] variable index is already used: %lu\n", index);
        fprintf(stderr, "[frame_set_variable] found value: ");
        value_print(found, stderr);
        fprintf(stderr, "\n");
        exit(1);
    }

    array_set(self->variable_array, index, value);
}
