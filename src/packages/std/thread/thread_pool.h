#pragma once

thread_pool_t *thread_pool_new(void);
void thread_pool_destroy(thread_pool_t **self_pointer);

size_t thread_pool_start(thread_pool_t *self, thread_fn_t *thread_fn, void *arg);
void thread_pool_join(thread_pool_t *self, size_t id);
void thread_pool_join_all(thread_pool_t *self);
