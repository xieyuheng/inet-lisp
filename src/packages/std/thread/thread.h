#pragma once

struct thread_t {
    tid_t tid;
};

tid_t thread_start(thread_fn_t *thread_fn, void *arg);
void *thread_join(tid_t tid);
