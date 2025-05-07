#include "index.h"

void
test_counter(void) {
    test_start();

    test_counter_stat();
    test_counter_stat_eventual();

    test_end();
}
