#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *kind(mode_t mode)
{
    if (S_ISREG(mode)) return "regular";
    if (S_ISDIR(mode)) return "directory";
    if (S_ISLNK(mode)) return "symlink";
    return "other";
}

static void print_stat(const char *label, const struct stat *info)
{
    printf("%s:type=%s inode=%llu size=%lld links=%llu mode=%04o\n",
           label,
           kind(info->st_mode),
           (unsigned long long)info->st_ino,
           (long long)info->st_size,
           (unsigned long long)info->st_nlink,
           (unsigned int)(info->st_mode & 07777U));
}

int main(int argc, char **argv)
{
    struct stat followed;
    struct stat link_info;
    struct stat fd_info;
    int fd;

    if (argc != 2) {
        fprintf(stderr, "usage: %s PATH\n", argv[0]);
        return 2;
    }

    if (stat(argv[1], &followed) != 0) {
        perror("stat");
        return 1;
    }

    if (lstat(argv[1], &link_info) != 0) {
        perror("lstat");
        return 1;
    }

    print_stat("stat", &followed);
    print_stat("lstat", &link_info);

    fd = open(argv[1], O_RDONLY);
    if (fd >= 0) {
        if (fstat(fd, &fd_info) == 0) {
            print_stat("fstat", &fd_info);
        }
        (void)close(fd);
    }

    return 0;
}
