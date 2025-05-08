#pragma once

struct array_t {
    size_t size;
    size_t grow_step;
    size_t cursor;
    void **values;
    destroy_fn_t *destroy_fn;
};

// growable array

array_t *array_new(size_t size);
void array_purge(array_t *self);
void array_destroy(array_t **self_pointer);

void array_set_destroy_fn(array_t *self, destroy_fn_t *destroy_fn);
array_t *array_new_with(size_t size, destroy_fn_t *destroy_fn);

#define ARRAY_AUTO_SIZE 64

array_t *array_new_auto(void);
array_t *array_new_auto_with(destroy_fn_t *destroy_fn);

inline size_t
array_size(const array_t *self) {
    return self->size;
}

inline size_t
array_grow_step(const array_t *self) {
    return self->grow_step;
}

inline void
array_set_grow_step(array_t *self, size_t grow_step) {
    self->grow_step = grow_step;
}

inline size_t
array_length(const array_t *self) {
    return self->cursor;
}

inline bool
array_is_empty(const array_t *self) {
    return self->cursor == 0;
}

inline bool
array_is_full(const array_t *self) {
    return self->cursor == self->size;
}

inline void
array_resize(array_t *self, size_t larger_size) {
    assert(larger_size >= self->size);
    if (larger_size == self->size) return;

    self->values = reallocate_pointers(self->values, self->size, larger_size);
    self->size = larger_size;
}

inline void *
array_top(array_t *self) {
    assert(self->cursor > 0);
    void *value = self->values[self->cursor - 1];
    return value;
}

inline void *
array_pop(array_t *self) {
    assert(self->cursor > 0);
    self->cursor--;
    void *value = self->values[self->cursor];
    self->values[self->cursor] = NULL;
    return value;
}

inline void
array_push(array_t *self, void *value) {
    if (array_is_full(self))
        array_resize(self, self->size + self->grow_step);

    self->values[self->cursor] = value;
    self->cursor++;
}

inline void *
array_get(const array_t *self, size_t index) {
    if (index >= self->size)
        return NULL;

    return self->values[index];
}

inline void *
array_pick(const array_t *self, size_t back_index) {
    assert(back_index < self->cursor);
    size_t index = self->cursor - 1 - back_index;
    return array_get(self, index);
}

inline void
array_set(array_t *self, size_t index, void *value) {
    if (index >= self->size)
        array_resize(self, index + self->grow_step);

    self->values[index] = value;

    if (index >= self->cursor)
        self->cursor = index + 1;
}
