#include "index.h"

void
test_counter(void) {
    test_start();

    test_counter_non_atomic();
    test_counter_atomic();
    test_counter_stat();
    test_counter_stat_eventual();

    test_end();
}
