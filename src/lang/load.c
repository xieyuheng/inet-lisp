#include "index.h"

hash_t *global_mod_cache = NULL;
node_allocator_t *global_node_allocator = NULL;

extern void import_prelude(mod_t *mod);

mod_t *
load_mod(path_t *path) {
    if (!global_mod_cache) {
        global_mod_cache = hash_of_string_key();
    }

    if (!global_node_allocator) {
        global_node_allocator = node_make_allocator();
     }

    mod_t *found_mod = hash_get(global_mod_cache, path_string(path));
    if (found_mod) {
        path_destroy(&path);
        return found_mod;
    }

    file_t *file = file_open_or_fail(path_string(path), "r");
    char *code = file_read_string(file);
    fclose(file);

    mod_t *mod = mod_new(path, code);
    import_prelude(mod);
    worker_t *loader_worker = worker_new(mod, global_node_allocator);

    list_t *sexp_list = sexp_parse_list(code) ;
    list_t *stmt_list = parse_stmt_list(sexp_list);
    stmt_t *stmt = list_first(stmt_list);
    while (stmt) {
        execute(loader_worker, stmt);
        stmt = list_next(stmt_list);
    }

    list_destroy(&stmt_list);
    list_destroy(&sexp_list);

    mod->loader_worker = loader_worker;

    char *key = string_copy(path_string(path));
    assert(hash_set(global_mod_cache, key, mod));

    return mod;
}

void
clear_global_mod_cache(void) {
    if (global_mod_cache) {
        hash_destroy(&global_mod_cache);
        global_mod_cache = hash_of_string_key();
    }

    if (global_node_allocator) {
        node_allocator_destroy(&global_node_allocator);
        global_node_allocator = node_make_allocator();
     }
}
