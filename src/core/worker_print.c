#include "index.h"

static void
worker_print_return_stack(const worker_t *self, file_t *file) {
    size_t return_stack_length = stack_length(self->return_stack);
    fprintf(file, "<return-stack length=\"%lu\">\n", return_stack_length);
    for (size_t i = 0; i < return_stack_length; i++) {
        frame_t *frame = stack_get(self->return_stack, i);
        frame_print(frame, file);
    }

    fprintf(file, "</return-stack>\n");
}

static void
worker_print_value_stack(const worker_t *self, file_t *file) {
    size_t value_stack_length = stack_length(self->value_stack);
    fprintf(file, "<value-stack length=\"%lu\">\n", value_stack_length);
    for (size_t i = 0; i < value_stack_length; i++) {
        value_t value = stack_get(self->value_stack, i);
        value_print(value, file);
        fprintf(file, "\n");
    }

    fprintf(file, "</value-stack>\n");
}

static void
worker_print_task_deque(const worker_t *self, file_t *file) {
    size_t task_count = deque_length(self->task_deque);
    fprintf(file, "<task-deque length=\"%lu\">\n", task_count);
    for (size_t i = 0; i < task_count; i++) {
        task_t *task = deque_get(self->task_deque, i);
        task_print(task, file);
        fprintf(file, "\n");
    }
    fprintf(file, "</task-deque>\n");
}

void
worker_print(const worker_t *self, file_t *file) {
    fprintf(file, "<worker>\n");
    worker_print_task_deque(self, file);
    worker_print_return_stack(self, file);
    worker_print_value_stack(self, file);
    fprintf(file, "</worker>\n");
}
