#include "sysutil/commands.h"
#include "sysutil/common.h"

#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>

int sysutil_cmd_stat(int argc, char **argv)
{
    if (argc != 2) {
        (void)fprintf(stderr, "usage: sysutil stat PATH\n");
        return 2;
    }

    struct stat info;
    if (lstat(argv[1], &info) != 0) {
        return sysutil_error_errno("lstat", argv[1]);
    }

    (void)printf("path=%s\n", argv[1]);
    (void)printf("type=%s\n", sysutil_file_type(info.st_mode));
    (void)printf("size=%jd\n", (intmax_t)info.st_size);
    (void)printf("mode=%04o\n", (unsigned int)(info.st_mode & 07777));
    (void)printf("inode=%ju\n", (uintmax_t)info.st_ino);
    (void)printf("links=%ju\n", (uintmax_t)info.st_nlink);

    return 0;
}
