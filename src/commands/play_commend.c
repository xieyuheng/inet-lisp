#include "index.h"

static int run(commander_t *commander);

void
play_command(commander_t *runner) {
    command_t *command = command_new("play");
    command->description = "play files";
    command->run = run;
    commander_add(runner, command);
}

int
run(commander_t *commander) {
    char **argv = commander_rest_argv(commander);
    argv = commander_rest_argv(commander);
    while (*argv) {
        char *src = *argv++;
        char *cwd = getcwd(NULL, 0);
        path_t *path = path_new(cwd);
        path_join(path, src);
        player_flag = true;
        mod_t *mod = load_mod(path);
        player_start(mod->loader_worker);
    }

    return 0;
}
