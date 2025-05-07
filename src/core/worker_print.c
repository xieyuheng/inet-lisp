#include "index.h"

static void
worker_print_return_stack(const worker_t *self, file_t *file) {
    size_t length = stack_length(self->return_stack);
    fprintf(file, " :return-stack-length %lu\n", length);
    fprintf(file, " :return-stack\n");
    fprintf(file, " (");
    for (size_t i = 0; i < length; i++) {
        frame_t *frame = stack_get(self->return_stack, i);
        if (i > 0) fprintf(file, "  ");
        frame_print_within_worker(frame, self, file);
        if (i != length - 1) fprintf(file, "\n");
    }
    fprintf(file, ")");
}

static void
worker_print_value_stack(const worker_t *self, file_t *file) {
    size_t length = stack_length(self->value_stack);
    fprintf(file, " :value-stack-length %lu\n", length);
    fprintf(file, " :value-stack\n");
    fprintf(file, " (");
    for (size_t i = 0; i < length; i++) {
        value_t value = stack_get(self->value_stack, i);
        if (i > 0) fprintf(file, "  ");
        worker_print_value(self, value, file);
        if (i != length - 1) fprintf(file, "\n");
    }
    fprintf(file, ")");
}

static void
worker_print_task_deque(const worker_t *self, file_t *file) {
    size_t length = deque_length(self->task_deque);
    fprintf(file, " :task-deque-length %lu\n", length);
    fprintf(file, " :task-deque\n");
    fprintf(file, " (");
    for (size_t i = 0; i < length; i++) {
        task_t *task = deque_get(self->task_deque, i);
        if (i > 0) fprintf(file, "  ");
        task_print(task, file);
        if (i != length - 1) fprintf(file, "\n");
    }
    fprintf(file, ")");
}

static void
worker_print_id(const worker_t *self, file_t *file) {
    if (worker_is_loader(self)) {
        fprintf(file, " :id loader");
    } else {
        fprintf(file, " :id %lu", self->id);
    }
}

void
worker_print(const worker_t *self, file_t *file) {
    fprintf(file, "(worker\n");
    worker_print_id(self, file); fprintf(file, "\n");
    worker_print_task_deque(self, file); fprintf(file, "\n");
    worker_print_return_stack(self, file); fprintf(file, "\n");
    worker_print_value_stack(self, file); fprintf(file, ")");
}
