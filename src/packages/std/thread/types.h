#pragma once

typedef struct thread_t thread_t;
typedef struct thread_pool_t thread_pool_t;

typedef void (thread_fn_t)(thread_t *thread);
