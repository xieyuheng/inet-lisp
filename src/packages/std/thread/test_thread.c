#include "index.h"

void
test_thread(void) {
    test_start();

    test_thread_start();

    test_thread_weak_memory_dekker();
    test_thread_weak_memory_dekker_relaxed();
    test_thread_weak_memory_dekker_atomic();

    test_end();
}
