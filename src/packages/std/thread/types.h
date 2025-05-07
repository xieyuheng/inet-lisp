#pragma once

typedef void *(thread_fn_t)(void *arg);

typedef struct thread_t thread_t;
typedef size_t thread_id_t;
typedef struct thread_pool_t thread_pool_t;
