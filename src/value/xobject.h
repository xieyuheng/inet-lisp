#pragma once

typedef void (print_fn_t)(const void *value, file_t *file);

struct object_spec_t {
    const char *name;
    print_fn_t *print_fn;
};

struct object_t {
    const object_spec_t *spec;
};

inline value_t
xobject(object_t *target) {
    return target;
}

inline bool
is_xobject(value_t value) {
    return value != NULL && value_tag(value) == XOBJECT;
}

inline object_t *
as_object(value_t value) {
    assert(is_xobject(value));
    return value;
}
