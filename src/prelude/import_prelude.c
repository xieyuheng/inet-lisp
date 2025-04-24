#include "index.h"

void
import_prelude(mod_t *mod) {
    // testing

    define_primitive_fn(mod, "assert", 1, 0, x_assert);

    // net

    define_primitive_fn(mod, "connect", 2, 0, x_connect);
    define_primitive_fn(mod, "link", 0, 2, x_link);

    // console

    define_primitive_fn_1(mod, "print", x_print);
    define_primitive_fn(mod, "newline", 0, 0, x_newline);
    define_primitive_fn_1(mod, "println", x_println);
}
