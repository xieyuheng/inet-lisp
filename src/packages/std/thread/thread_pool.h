#pragma once

thread_pool_t *thread_pool_new(void);
void thread_pool_destroy(thread_pool_t **self_pointer);
