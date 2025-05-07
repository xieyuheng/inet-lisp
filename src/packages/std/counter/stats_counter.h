#pragma once

// to provide statistics to human,
// fast per-thread write,
// slow total read.

struct stats_counter_t {
    size_t size;
    array_t *count_pointer_array;
};

stats_counter_t *stats_counter_new(size_t size);
