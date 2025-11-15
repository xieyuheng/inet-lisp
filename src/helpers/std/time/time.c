#include "index.h"

double
time_second(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    double second = ts.tv_sec + ts.tv_nsec * 1e-9;
    return second;
}

double
time_passed_second(double start_second) {
    double end_second = time_second();
    return end_second - start_second;
}

uint64_t
time_nanosecond(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint64_t nanosecond = ts.tv_sec * 1e+9 + ts.tv_nsec;
    return nanosecond;
}

uint64_t
time_passed_nanosecond(uint64_t start_nanosecond) {
    uint64_t end_nanosecond = time_nanosecond();
    return end_nanosecond - start_nanosecond;
}

bool
time_sleep_nanosecond(long nanosecond) {
    struct timespec ts = {
        .tv_sec = 0,
        .tv_nsec = nanosecond,
    };
    int ok = nanosleep(&ts, NULL);
    return ok == 0;
}
