#include "index.h"

static int run(commander_t *commander);

void
cmd_benchmark(commander_t *runner) {
    command_t *command = command_new("benchmark");
    command->description = "benchmark files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **argv = commander_rest_argv(commander);
    while (*argv) {
        char *arg = *argv++;

        double sequential_second = 0;
        double parallel_second = 0;

        {
            single_threaded_flag = true;
            double start_second = time_second();
            path_t *path = path_new_cwd();
            path_resolve(path, arg);
            load_mod(path);
            sequential_second = time_passed_second(start_second);
        }

        clear_global_mod_cache();

        {
            single_threaded_flag = false;
            double start_second = time_second();
            path_t *path = path_new_cwd();
            path_resolve(path, arg);
            load_mod(path);
            parallel_second = time_passed_second(start_second);
        }

        double speedup = sequential_second / parallel_second;
        printf("[benchmark] speedup: %.2f x, %s\n", speedup, arg);
    }

    return 0;
}
