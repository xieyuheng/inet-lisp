#include "index.h"

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
    self->variable_array = array_new_auto();
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
    fprintf(file, "(frame ");
    function_print_with_cursor(self->function, file, self->cursor);

    fprintf(file, "\n   ");
    fprintf(file, ":local-variable-array ");
    fprintf(file, "(");
    size_t length = array_length(self->variable_array);
    for (size_t i = 0; i < length; i++) {
        value_t value = array_get(self->variable_array, i);
        if (value) value_print(walk(value), file);
        else fprintf(file, "____");

        if (i != length - 1) fprintf(file, " ");
    }

    fprintf(file, ")");
    fprintf(file, ")");
}

void
frame_print_within_worker(const frame_t *self, const worker_t *worker, file_t *file) {
    fprintf(file, "(frame ");
    function_print_with_cursor(self->function, file, self->cursor);

    fprintf(file, "\n   ");
    fprintf(file, ":local-variable-array ");
    fprintf(file, "(");
    size_t length = array_length(self->variable_array);
    for (size_t i = 0; i < length; i++) {
        value_t value = array_get(self->variable_array, i);
        if (value) worker_print_value(worker, value, file);
        else fprintf(file, "____");

        if (i != length - 1) fprintf(file, " ");
    }

    fprintf(file, ")");
    fprintf(file, ")");
}

value_t
frame_get_variable(const frame_t *self, size_t index) {
    value_t value = array_get(self->variable_array, index);
    if (!value) {
        who_printf("undefined variable index: %lu\n", index);
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
        who_printf("variable index is already used: %lu\n", index);
        who_printf("found value: ");
        value_print(found, stderr);
        fprintf(stderr, "\n");
        exit(1);
    }

    array_set(self->variable_array, index, value);
}
