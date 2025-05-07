#include "deps.h"
#include "commands/index.h"

int
main(int argc, char *argv[]) {
    file_disable_buffer(stdout);
    file_disable_buffer(stderr);

    set_cache_line_size(CACHE_LINE_SIZE);

    commander_t *commander = commander_new("inet-lisp", INET_LISP_ST_VERSION, argc, argv);

    commander_use(commander, cmd_run);
    commander_use(commander, cmd_info);
    commander_use(commander, cmd_benchmark);
    commander_use(commander, cmd_test_self);
    commander_use(commander, cmd_test_packages);
    commander_use(commander, cmd_default_version);
    commander_use(commander, cmd_default_help);

    return commander_run(commander);
}
