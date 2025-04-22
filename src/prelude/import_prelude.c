#include "index.h"

void
import_prelude(mod_t *mod) {
    // bool

    define(mod, "false", xfalse);
    define(mod, "true", xtrue);
    define_primitive_fn_node_1(mod, "not", xbool_not, (const char*[]) { "x!", "result" });
    define_primitive_fn_node_2(mod, "and", xbool_and, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "or", xbool_or, (const char*[]) { "x!", "y!", "result" });

    // value

    define_primitive_fn_node_2(mod, "eq?", x_eq, (const char*[]) { "x!", "y!", "result" });

    // testing

    define_primitive_fn(mod, "assert", 1, 0, x_assert);

    // int

    define_primitive_fn_node_1(mod, "int?", xint_p, (const char*[]) { "x!", "result" });
    define_primitive_fn_node_2(mod, "iadd", xint_add, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "isub", xint_sub, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "imul", xint_mul, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "idiv", xint_div, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "imod", xint_mod, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_1(mod, "int-to-float", xint_to_xfloat, (const char*[]) { "i!", "f" });
    define_primitive_fn_node(mod, "int-dup", 1, 2, xint_dup, (const char*[]) { "target!", "first", "second" });

    // float

    define_primitive_fn_node_1(mod, "float?", xfloat_p, (const char*[]) { "x!", "result" });
    define_primitive_fn_node_2(mod, "fadd", xfloat_add, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "fsub", xfloat_sub, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "fmul", xfloat_mul, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "fdiv", xfloat_div, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_2(mod, "fmod", xfloat_mod, (const char*[]) { "x!", "y!", "result" });
    define_primitive_fn_node_1(mod, "float-to-int", xfloat_to_xint, (const char*[]) { "f!", "i" });
    define_primitive_fn_node(mod, "float-dup", 1, 2, xfloat_dup, (const char*[]) { "target!", "first", "second" });

    // net

    define_primitive_fn(mod, "connect", 2, 0, x_connect);
    define_primitive_fn(mod, "wire-print-net", 1, 1, x_wire_print_net);
    define_primitive_fn(mod, "link", 0, 2, x_link);
    define_primitive_fn(mod, "run", 0, 0, x_run);
    define_primitive_fn(mod, "inspect-run", 1, 1, x_inspect_run);

    // console

    define_primitive_fn_1(mod, "print", x_print);
    define_primitive_fn(mod, "newline", 0, 0, x_newline);
    define_primitive_fn_1(mod, "println", x_println);

    // function

    define_primitive_fn_node(mod, "fn-dup", 1, 2, x_fn_dup, (const char*[]) { "target!", "first", "second" });
}
