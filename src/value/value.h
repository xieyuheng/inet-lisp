#pragma once

inline tag_t
value_tag(value_t value) {
    return (size_t) value & TAG_MASK;
}

void value_print(value_t value, file_t *file);
