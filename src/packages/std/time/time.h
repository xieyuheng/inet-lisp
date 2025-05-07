#pragma once

double time_second(void);
double time_passed_second(double start_second);

uint64_t time_nanosecond(void);
uint64_t time_passed_nanosecond(uint64_t start_nanosecond);
bool time_sleep_nanosecond(long nanosecond);
