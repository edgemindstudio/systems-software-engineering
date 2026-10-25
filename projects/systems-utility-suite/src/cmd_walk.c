#include "sysutil/commands.h"
#include "sysutil/common.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static int join_path(const char *left, const char *right, char **out)
{
    const size_t left_len = strlen(left);
    const size_t right_len = strlen(right);
    const int separator = left_len > 0U && left[left_len - 1U] != '/';
    const size_t total = left_len + (separator ? 1U : 0U) + right_len + 1U;

    char *result = malloc(total);
    if (result == NULL) {
        return -1;
    }

    const int written = snprintf(result, total, "%s%s%s",
                                 left, separator ? "/" : "", right);
    if (written < 0 || (size_t)written >= total) {
        free(result);
        errno = EOVERFLOW;
        return -1;
    }

    *out = result;
    return 0;
}

static int walk_path(const char *path, unsigned int depth)
{
    struct stat info;
    if (lstat(path, &info) != 0) {
        return sysutil_error_errno("lstat", path);
    }

    (void)printf("depth=%u type=%s path=%s\n",
                 depth, sysutil_file_type(info.st_mode), path);

    if (!S_ISDIR(info.st_mode)) {
        return 0;
    }

    DIR *dir = opendir(path);
    if (dir == NULL) {
        return sysutil_error_errno("opendir", path);
    }

    int result = 0;
    errno = 0;

    for (;;) {
        struct dirent *entry = readdir(dir);
        if (entry == NULL) {
            if (errno != 0) {
                result = sysutil_error_errno("readdir", path);
            }
            break;
        }

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char *child = NULL;
        if (join_path(path, entry->d_name, &child) != 0) {
            result = sysutil_error_errno("construct path", path);
            break;
        }

        result = walk_path(child, depth + 1U);
        free(child);

        if (result != 0) {
            break;
        }

        errno = 0;
    }

    if (closedir(dir) != 0 && result == 0) {
        result = sysutil_error_errno("closedir", path);
    }

    return result;
}

int sysutil_cmd_walk(int argc, char **argv)
{
    if (argc != 2) {
        (void)fprintf(stderr, "usage: sysutil walk DIRECTORY\n");
        return 2;
    }

    struct stat info;
    if (lstat(argv[1], &info) != 0) {
        return sysutil_error_errno("lstat", argv[1]);
    }

    if (!S_ISDIR(info.st_mode)) {
        (void)fprintf(stderr, "sysutil: walk target is not a directory: '%s'\n",
                      argv[1]);
        return 1;
    }

    return walk_path(argv[1], 0U);
}
