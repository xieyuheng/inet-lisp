#include "index.h"

void
worker_apply_wire(worker_t *worker, wire_t *wire, size_t arity) {
    value_t defuzed = defuze(wire);
    if (!is_wire(defuzed)) {
        worker_apply(worker, defuzed, arity);
        return;
    }

    assert(1 < arity && arity <= 4);
    who_printf("can not apply unfuzed wire\n");
    exit(1);
}
