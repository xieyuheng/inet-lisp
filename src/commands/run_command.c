#include "index.h"

static int run(commander_t *commander);

void
run_command(commander_t *runner) {
    command_t *command = command_new("run");
    command->description = "run files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **argv = commander_rest_argv(commander);
    while (*argv) {
        char *arg = *argv++;
        if (string_equal(arg, "--single-threaded") || string_equal(arg, "-s")) {
            single_threaded_flag = true;
            continue;
        }

        if (string_equal(arg, "--print-top-level-exp") || string_equal(arg, "-p")) {
            print_top_level_exp_flag = true;
            continue;
        }

        path_t *path = path_new_cwd();
        path_join(path, arg);
        load_mod(path);
    }

    return 0;
}
