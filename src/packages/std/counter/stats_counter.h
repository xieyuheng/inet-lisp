#pragma once

// to provide statistics to human,
// fast per-thread write,
// slow total read.

stats_counter_t *stats_counter_new(size_t size);
void stats_counter_destroy(stats_counter_t **self_pointer);

void stats_counter_per_thread_add1(stats_counter_t *self, size_t id);
void stats_counter_per_thread_sub1(stats_counter_t *self, size_t id);

int64_t stats_counter_total(stats_counter_t *self);
bool stats_counter_total_is_zero(stats_counter_t *self);
