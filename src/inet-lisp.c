#include "deps.h"
#include "commands/index.h"

int
main(int argc, char *argv[]) {
    file_disable_buffer(stdout);
    file_disable_buffer(stderr);

    commander_t *commander = commander_new("inet-lisp", INET_LISP_ST_VERSION, argc, argv);

    commander_use(commander, run_command);
    commander_use(commander, info_command);
    commander_use(commander, test_self_command);
    commander_use(commander, test_modules_command);
    commander_use(commander, default_version_command);
    commander_use(commander, default_help_command);

    return commander_run(commander);
}
