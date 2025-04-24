#include "index.h"

static wire_t *
connect_wire(worker_t* worker, wire_t *first_wire, wire_t *second_wire) {
    value_t first_opposite = first_wire->opposite;
    value_t second_opposite = second_wire->opposite;

    if (is_wire(first_opposite) && is_wire(second_opposite)) {
        wire_t *first_opposite_wire = as_wire(first_opposite);
        wire_t *second_opposite_wire = as_wire(second_opposite);

        first_opposite_wire->opposite = second_opposite_wire;
        second_opposite_wire->opposite = first_opposite_wire;

        wire_destroy(&first_wire);
        wire_destroy(&second_wire);

        worker_maybe_add_active_wire(worker, first_opposite_wire);

        return first_opposite_wire;
    } else if (is_wire(first_opposite)) {
        wire_t *first_opposite_wire = as_wire(first_opposite);
        first_opposite_wire->opposite = second_opposite;

        wire_destroy(&first_wire);
        wire_destroy(&second_wire);

        worker_maybe_add_active_wire(worker, first_opposite_wire);
        
        return first_opposite_wire;
    } else if (is_wire(second_opposite)) {
        wire_t *second_opposite_wire = as_wire(second_opposite);
        second_opposite_wire->opposite = first_opposite;

        wire_destroy(&first_wire);
        wire_destroy(&second_wire);

        worker_maybe_add_active_wire(worker, second_opposite_wire);
        
        return second_opposite_wire;
    } else {
        fprintf(stderr, "[connect_wire] can not connect wires with non-wire opposite\n");
        fprintf(stderr, "[connect_wire] first_opposite: ");
        value_print(first_opposite, stderr);
        fprintf(stderr, "\n");
        fprintf(stderr, "[connect_wire] second_opposite: ");
        value_print(second_opposite, stderr);
        fprintf(stderr, "\n");
        exit(1);
    }
}

wire_t *
worker_connect(worker_t* worker, wire_t *wire, value_t value) {
    if (is_wire(value)) {
        return connect_wire(worker, wire, value);
    }

     value_t opposite = wire->opposite;
     if (is_wire(opposite)) {
         wire_t *opposite_wire = as_wire(wire->opposite);
         opposite_wire->opposite = value;

        wire_destroy(&wire);

        worker_maybe_add_active_wire(worker, opposite_wire);
         
         return opposite_wire;
     } else {
        fprintf(stderr, "[worker_connect] can not connect wire with non-wire opposite to value\n");
        fprintf(stderr, "[worker_connect] opposite: ");
        value_print(opposite, stderr);
        fprintf(stderr, "\n");
        exit(1);
    }
}
