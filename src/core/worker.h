#pragma once

// A worker is like a little forth inner interpreter.

struct worker_t {
    mod_t *mod;
    list_t *task_list;
    stack_t *value_stack;
    stack_t *return_stack;
    size_t node_id_count;
    size_t fresh_name_count;
};

worker_t *worker_new(mod_t *mod);
void worker_destroy(worker_t **self_pointer);

node_t *worker_new_node(worker_t* self, const node_ctor_t *ctor);
void worker_recycle_node(worker_t* self, node_t *node);

char *worker_fresh_name(worker_t* self);

void worker_apply(worker_t *worker, value_t target, size_t arity);
void worker_run_until(worker_t *worker, size_t base_length);

void worker_maybe_add_active_wire(worker_t *worker, value_t value);
wire_t *worker_connect(worker_t* worker, wire_t *wire, value_t value);
void worker_add_task(worker_t* self, task_t *task);
void worker_handle_task(worker_t *worker, task_t *task);
void worker_disconnect_node(worker_t *worker, node_t *node);
void worker_reconnect_node(worker_t *worker, node_t *node, size_t arity);
void worker_work(worker_t *worker);

void worker_print_return_stack(const worker_t *self, file_t *file);
void worker_print_value_stack(const worker_t *self, file_t *file);
void worker_print(const worker_t *self, file_t *file);
