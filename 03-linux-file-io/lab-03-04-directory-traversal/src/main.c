#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static const char *kind(mode_t mode)
{
    if (S_ISREG(mode)) return "regular";
    if (S_ISDIR(mode)) return "directory";
    if (S_ISLNK(mode)) return "symlink";
    return "other";
}

int main(int argc, char **argv)
{
    DIR *directory;
    struct dirent *entry;

    if (argc != 2) {
        fprintf(stderr, "usage: %s DIRECTORY\n", argv[0]);
        return 2;
    }

    directory = opendir(argv[1]);
    if (directory == NULL) {
        fprintf(stderr, "opendir: %s\n", strerror(errno));
        return 1;
    }

    while ((entry = readdir(directory)) != NULL) {
        char path[PATH_MAX];
        struct stat info;
        int written;

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        written = snprintf(path, sizeof path, "%s/%s", argv[1], entry->d_name);
        if (written < 0 || (size_t)written >= sizeof path) {
            fprintf(stderr, "path too long: %s\n", entry->d_name);
            continue;
        }

        if (lstat(path, &info) != 0) {
            fprintf(stderr, "lstat %s: %s\n", path, strerror(errno));
            continue;
        }

        printf("%s\t%s\n", entry->d_name, kind(info.st_mode));
    }

    if (closedir(directory) != 0) {
        perror("closedir");
        return 1;
    }

    return 0;
}
