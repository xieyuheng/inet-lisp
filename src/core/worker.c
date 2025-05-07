#include "index.h"

worker_t *
worker_new(mod_t *mod, node_allocator_t *node_allocator) {
    worker_t *self = new(worker_t);
    self->mod = mod;
    self->task_queue = queue_new(WORKER_TASK_QUEUE_SIZE);
    self->task_queue_front_lock = fast_spinlock_new();
    // TODO We should use value_destroy to create value_stack.
    self->value_stack = stack_new();
    self->return_stack = stack_new_with((destroy_fn_t *) frame_destroy);
    self->node_allocator = node_allocator;
    self->free_node_stack = stack_new();
    return self;
}

void
worker_destroy(worker_t **self_pointer) {
    assert(self_pointer);
    if (*self_pointer == NULL) return;

    worker_t *self = *self_pointer;
    queue_destroy(&self->task_queue);
    fast_spinlock_destroy(&self->task_queue_front_lock);
    stack_destroy(&self->value_stack);
    stack_destroy(&self->return_stack);
    free(self);
    *self_pointer = NULL;
}

#if DEBUG_NODE_ALLOCATOR_DISABLED
atomic_size_t atomic_node_count = 0;
#endif

node_t *
worker_new_node(worker_t* self, const node_ctor_t *ctor) {
#if DEBUG_NODE_ALLOCATOR_DISABLED
    (void) self;
    node_t *node = node_new();
    node->ctor = ctor;
    node->id = atomic_fetch_add(&atomic_node_count, 1);
    return node;
#else
    node_t *node = node_allocator_allocate(self->node_allocator, self->free_node_stack);
    node->ctor = ctor;
    return node;
#endif
}

void
worker_recycle_node(worker_t* self, node_t *node) {
#if DEBUG_NODE_ALLOCATOR_DISABLED
    (void) self;
    node_clean(node);

    // NOTE To exclude ABA problem
    // we do not even free the memory of node.
    // node_destroy(&node);
#else
    node_clean(node);
    node_allocator_recycle(self->node_allocator, self->free_node_stack, &node);
#endif
}

bool
worker_is_loader(const worker_t *self) {
    return !self->scheduler;
}

node_t *
worker_lookup_node_by_wire(const worker_t* self, wire_t *wire) {
    array_t *node_array = allocated_node_array(self->node_allocator);
    for (size_t i = 0; i < array_length(node_array); i++) {

        node_t *node = array_get(node_array, i);
        if (node_has_wire(node, wire))
            return node;
    }

    return NULL;
}
