#include "index.h"

void
import_prelude(mod_t *mod) {
    // bool

    define(mod, "false", xfalse);
    define(mod, "true", xtrue);
    define_primitive_node_ctor_1(mod, "not", xbool_not, (const char*[]) { "x!", "result" });
    define_primitive_node_ctor_2(mod, "and", xbool_and, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "or", xbool_or, (const char*[]) { "x!", "y!", "result" });

    // value

    define_primitive_node_ctor_2(mod, "eq?", x_eq, (const char*[]) { "x!", "y!", "result" });

    // testing

    define_primitive_node_ctor(mod, "assert", 1, 0, x_assert, (const char*[]) { "value!" });

    // int

    define_primitive_node_ctor_1(mod, "int?", xint_p, (const char*[]) { "x!", "result" });
    define_primitive_node_ctor_2(mod, "iadd", xint_add, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "isub", xint_sub, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "imul", xint_mul, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "idiv", xint_div, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "imod", xint_mod, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_1(mod, "int-to-float", xint_to_xfloat, (const char*[]) { "i!", "f" });
    define_primitive_node_ctor(mod, "int-dup", 1, 2, xint_dup, (const char*[]) { "target!", "first", "second" });

    // float

    define_primitive_node_ctor_1(mod, "float?", xfloat_p, (const char*[]) { "x!", "result" });
    define_primitive_node_ctor_2(mod, "fadd", xfloat_add, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "fsub", xfloat_sub, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "fmul", xfloat_mul, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "fdiv", xfloat_div, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_2(mod, "fmod", xfloat_mod, (const char*[]) { "x!", "y!", "result" });
    define_primitive_node_ctor_1(mod, "float-to-int", xfloat_to_xint, (const char*[]) { "f!", "i" });
    define_primitive_node_ctor(mod, "float-dup", 1, 2, xfloat_dup, (const char*[]) { "target!", "first", "second" });

    // net

    define_primitive_fn(mod, "connect", 2, 0, x_connect);
    define_primitive_fn(mod, "link", 0, 2, x_link);

    // console

    define_primitive_fn(mod, "newline", 0, 0, x_newline);
    define_primitive_node_ctor(mod, "print", 1, 1, x_print, (const char*[]) { "value!", "result" });
    define_primitive_node_ctor(mod, "println", 1, 1, x_println, (const char*[]) { "value!", "result" });

    // function

    define_primitive_node_ctor(mod, "fn-dup", 1, 2, x_fn_dup, (const char*[]) { "target!", "first", "second" });
}
