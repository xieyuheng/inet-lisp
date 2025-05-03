#include "index.h"

inline static void
worker_execute_opcode(worker_t *worker, frame_t *frame, opcode_t *opcode) {
    switch (opcode->kind) {
    case OPCODE_APPLY: {
        value_t target = stack_pop(worker->value_stack);
        worker_apply(worker, target, opcode->apply.arity);
        return;
    }

    case OPCODE_LITERAL: {
        stack_push(worker->value_stack, opcode->literal.value);
        return;
    }

    case OPCODE_GET_VARIABLE: {
        value_t value = frame_get_variable(frame, opcode->get_variable.index);
        stack_push(worker->value_stack, value);
        return;
    }

    case OPCODE_SET_VARIABLE: {
        value_t value = stack_pop(worker->value_stack);
        frame_set_variable(frame, opcode->set_variable.index, value);
        return;
    }
    }
}

inline static void
worker_run_one_step(worker_t *worker) {
    if (stack_is_empty(worker->return_stack)) return;

    frame_t *frame = stack_pop(worker->return_stack);
    if (frame_is_finished(frame)) {
        frame_destroy(&frame);
        return;
    }

    opcode_t *op = frame_fetch_opcode(frame);

#if PROPER_TAIL_CALL_DISABLED
    stack_push(worker->return_stack, frame);
#else
    // proper tail-call = do not push finished frame.
    bool finished = frame_is_finished(frame);
    if (!finished) {
        stack_push(worker->return_stack, frame);
    }
#endif

    worker_execute_opcode(worker, frame, op);

#if PROPER_TAIL_CALL_DISABLED
#else
    if (finished) {
        frame_destroy(&frame);
    }
#endif
}

static void
worker_step_log(worker_t *worker, size_t return_stack_base, uint64_t start_at, size_t step) {
    if (!single_threaded_flag) {
        who_printf("can not do step log without single_threaded_flag");
        exit(1);
    }

    file_lock(stdout);
    who_printf("%s", "");
    printf("return_stack_base: %lu", return_stack_base); fprintf(stdout, ", ");
    printf("start_at: %lu", start_at); fprintf(stdout, ", ");
    printf("step: %lu", step); fprintf(stdout, "\n");
    worker_print(worker, stdout); fprintf(stdout, "\n");
    file_unlock(stdout);
}

void
worker_run_until(worker_t *worker, size_t return_stack_base) {
#if DEBUG_STEP_LOG
    size_t step = 0;
    uint64_t start_at = time_nanosecond();
    worker_step_log(worker, return_stack_base, start_at, step++);
#else
    (void) worker_step_log;
#endif

    while (stack_length(worker->return_stack) > return_stack_base) {
        worker_run_one_step(worker);

#if DEBUG_STEP_LOG
        worker_step_log(worker, return_stack_base, start_at, step++);
#endif
    }
}
