#include "index.h"

static int run(commander_t *commander);

void
cmd_info(commander_t *commander) {
    command_t *command = make_command("info");
    command->description = "print system info";
    command->run = run;
    commander_add(commander, command);
}

int
run(commander_t *commander) {
    (void) commander;

    printf("page size: %ld bytes\n", sysconf(_SC_PAGE_SIZE));

#if __GLIBC__
    printf("l1 cache size: %ld bytes\n", sysconf(_SC_LEVEL1_DCACHE_SIZE));
    printf("l1 cache line size: %ld bytes\n", sysconf(_SC_LEVEL1_DCACHE_LINESIZE));

    printf("l2 cache size: %ld bytes\n", sysconf(_SC_LEVEL2_CACHE_SIZE));
    printf("l2 cache line size: %ld bytes\n", sysconf(_SC_LEVEL2_CACHE_LINESIZE));

    printf("l3 cache size: %ld bytes\n", sysconf(_SC_LEVEL3_CACHE_SIZE));
    printf("l3 cache line size: %ld bytes\n", sysconf(_SC_LEVEL3_CACHE_LINESIZE));
#endif

    printf("number of processors: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("size of time_t: %ld bits\n", sizeof(time_t) * 8);

    return 0;
}
