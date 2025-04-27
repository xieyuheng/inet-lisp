#include "index.h"

void
worker_print_value(const worker_t *self, value_t value, file_t *file) {
    if (!is_wire(value)) {
        value_print(value, file);
        return;
    }

    wire_t *wire = as_wire(value);
    node_t *node = worker_lookup_node_by_wire(self, wire);
    if (!node) {
        value_print(value, file);
        return;
    }

    size_t index = node_wire_index(node, wire);
    fprintf(file, "(wire ");
    fprintf(file, "(");
    node_print_port_info(node, index, file);
    fprintf(file, " ");
    node_print(node, file);
    fprintf(file, ")");
    fprintf(file, ")");
}
