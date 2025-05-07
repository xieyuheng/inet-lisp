#pragma once

// A worker is like a little forth inner interpreter.

struct worker_t {
    mod_t *mod;
    deque_t *task_deque;
    stack_t *value_stack;
    stack_t *return_stack;
    node_allocator_t *node_allocator;
    stack_t *free_node_stack;
    scheduler_t *scheduler;
    size_t id;
    size_t victim_cursor;
};

worker_t *worker_new(mod_t *mod, node_allocator_t *node_allocator);
void worker_destroy(worker_t **self_pointer);

void worker_apply_primitive(worker_t *worker, const primitive_t *primitive);
void worker_apply_wire(worker_t *worker, wire_t *wire, size_t arity);
void worker_apply(worker_t *worker, value_t target, size_t arity);
void worker_run_until(worker_t *worker, size_t return_stack_base);

node_t *worker_new_node(worker_t* self, const node_ctor_t *ctor);
void worker_recycle_node(worker_t* self, node_t *node);

void worker_connect(worker_t *worker, value_t left, value_t right);
void worker_add_task(worker_t *worker, task_t *task);
task_t *worker_next_task(worker_t *worker);
task_t *worker_offer_task(worker_t *worker);
void worker_handle_task(worker_t *worker, task_t *task);
void worker_disconnect_node(worker_t *worker, node_t *node);
void worker_reconnect_node(worker_t *worker, node_t *node, size_t arity);
void worker_work_sequentially(worker_t *worker);
void worker_work_parallelly(worker_t *worker);
void worker_work(worker_t *worker);

node_t *worker_lookup_node_by_wire(const worker_t* self, wire_t *wire);
void worker_print_value(const worker_t *self, value_t value, file_t *file);
void worker_print(const worker_t *self, file_t *file);

bool worker_is_loader(const worker_t *self);
