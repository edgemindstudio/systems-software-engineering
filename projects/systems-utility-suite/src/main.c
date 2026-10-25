#include "sysutil/commands.h"

#include <stdio.h>
#include <string.h>

static void print_usage(FILE *stream)
{
    (void)fprintf(stream,
        "usage:\n"
        "  sysutil stat PATH\n"
        "  sysutil copy SOURCE DESTINATION\n"
        "  sysutil walk DIRECTORY\n"
        "  sysutil hex FILE [MAX_BYTES]\n"
        "  sysutil proc [PID|self]\n"
        "  sysutil help\n");
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        print_usage(stderr);
        return 2;
    }

    const char *command = argv[1];

    if (strcmp(command, "stat") == 0) {
        return sysutil_cmd_stat(argc - 1, argv + 1);
    }
    if (strcmp(command, "copy") == 0) {
        return sysutil_cmd_copy(argc - 1, argv + 1);
    }
    if (strcmp(command, "walk") == 0) {
        return sysutil_cmd_walk(argc - 1, argv + 1);
    }
    if (strcmp(command, "hex") == 0) {
        return sysutil_cmd_hex(argc - 1, argv + 1);
    }
    if (strcmp(command, "proc") == 0) {
        return sysutil_cmd_proc(argc - 1, argv + 1);
    }
    if (strcmp(command, "help") == 0 ||
        strcmp(command, "--help") == 0 ||
        strcmp(command, "-h") == 0) {
        print_usage(stdout);
        return 0;
    }

    (void)fprintf(stderr, "sysutil: unknown command '%s'\n", command);
    print_usage(stderr);
    return 2;
}
