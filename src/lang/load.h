#pragma once

extern hash_t *global_mod_cache;

mod_t *load_mod(path_t *path);

void clear_global_mod_cache(void);
