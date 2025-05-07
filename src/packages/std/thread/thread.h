#pragma once

thread_t *thread_start(thread_fn_t *thread_fn, void *arg);
void *thread_join(thread_t *self);

bool thread_is_in_pool(thread_t *self);
size_t thread_id(thread_t *self);
