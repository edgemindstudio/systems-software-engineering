#include "sysutil/commands.h"
#include "sysutil/common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int valid_pid_text(const char *text)
{
    if (strcmp(text, "self") == 0) {
        return 1;
    }

    if (*text == '\0') {
        return 0;
    }

    for (const unsigned char *p = (const unsigned char *)text;
         *p != '\0'; ++p) {
        if (*p < (unsigned char)'0' || *p > (unsigned char)'9') {
            return 0;
        }
    }

    return 1;
}

static int wanted_field(const char *line)
{
    return strncmp(line, "Name:", 5U) == 0 ||
           strncmp(line, "State:", 6U) == 0 ||
           strncmp(line, "Threads:", 8U) == 0 ||
           strncmp(line, "VmRSS:", 6U) == 0;
}

int sysutil_cmd_proc(int argc, char **argv)
{
    if (argc > 2) {
        (void)fprintf(stderr, "usage: sysutil proc [PID|self]\n");
        return 2;
    }

    const char *pid = argc == 2 ? argv[1] : "self";
    if (!valid_pid_text(pid)) {
        (void)fprintf(stderr, "sysutil: invalid PID '%s'\n", pid);
        return 2;
    }

    char path[128];
    const int count = snprintf(path, sizeof path, "/proc/%s/status", pid);
    if (count < 0 || (size_t)count >= sizeof path) {
        errno = EOVERFLOW;
        return sysutil_error_errno("construct proc path", pid);
    }

    FILE *stream = fopen(path, "r");
    if (stream == NULL) {
        return sysutil_error_errno("fopen", path);
    }

    (void)printf("pid=%s\n", pid);

    char *line = NULL;
    size_t capacity = 0U;
    int fields = 0;

    while (getline(&line, &capacity, stream) >= 0) {
        if (wanted_field(line)) {
            (void)fputs(line, stdout);
            ++fields;
        }
    }

    free(line);

    if (ferror(stream) != 0) {
        const int saved = errno;
        (void)fclose(stream);
        errno = saved != 0 ? saved : EIO;
        return sysutil_error_errno("read", path);
    }

    if (fclose(stream) != 0) {
        return sysutil_error_errno("fclose", path);
    }

    (void)printf("reported_fields=%d\n", fields);
    return fields >= 3 ? 0 : 1;
}
