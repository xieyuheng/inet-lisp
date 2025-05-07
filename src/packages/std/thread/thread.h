#pragma once

thread_t *thread_start(thread_fn_t *thread_fn, void *arg);
void *thread_join(thread_t *self);
