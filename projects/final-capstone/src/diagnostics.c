#include "sysdiag/diagnostics.h"

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int appendf(char *buffer,
                   size_t capacity,
                   size_t *used,
                   const char *format,
                   ...)
{
    if (*used >= capacity) {
        return -1;
    }

    va_list ap;
    va_start(ap, format);
    int n = vsnprintf(buffer + *used,
                      capacity - *used,
                      format,
                      ap);
    va_end(ap);

    if (n < 0 || (size_t)n >= capacity - *used) {
        return -1;
    }

    *used += (size_t)n;
    return 0;
}

static const char *file_type(mode_t mode)
{
    if (S_ISREG(mode)) return "regular";
    if (S_ISDIR(mode)) return "directory";
    if (S_ISLNK(mode)) return "symlink";
    if (S_ISCHR(mode)) return "character";
    if (S_ISBLK(mode)) return "block";
    if (S_ISFIFO(mode)) return "fifo";
    if (S_ISSOCK(mode)) return "socket";
    return "other";
}

static int handle_stat(const char *path,
                       char *response,
                       size_t capacity)
{
    struct stat st;
    if (path == NULL || *path == '\0') {
        (void)snprintf(response, capacity,
                       "ERR code=EINVAL message=missing-path\n");
        return 1;
    }

    if (lstat(path, &st) != 0) {
        (void)snprintf(response, capacity,
                       "ERR code=%d message=%s\n",
                       errno, strerror(errno));
        return 1;
    }

    (void)snprintf(response, capacity,
                   "OK command=STAT\n"
                   "path=%s\n"
                   "type=%s\n"
                   "size=%" PRIuMAX "\n"
                   "mode=%04o\n"
                   "inode=%" PRIuMAX "\n"
                   "links=%" PRIuMAX "\n",
                   path,
                   file_type(st.st_mode),
                   (uintmax_t)st.st_size,
                   (unsigned)(st.st_mode & 07777U),
                   (uintmax_t)st.st_ino,
                   (uintmax_t)st.st_nlink);
    return 0;
}

static int handle_proc(const char *target,
                       char *response,
                       size_t capacity)
{
    char path[128];

    if (target == NULL || *target == '\0' ||
        strcmp(target, "self") == 0) {
        (void)snprintf(path, sizeof path, "/proc/self/status");
    } else {
        char *end = NULL;
        errno = 0;
        long pid = strtol(target, &end, 10);
        if (errno != 0 || end == target ||
            *end != '\0' || pid <= 0L) {
            (void)snprintf(response, capacity,
                           "ERR code=EINVAL message=invalid-pid\n");
            return 1;
        }
        (void)snprintf(path, sizeof path,
                       "/proc/%ld/status", pid);
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        (void)snprintf(response, capacity,
                       "ERR code=%d message=%s\n",
                       errno, strerror(errno));
        return 1;
    }

    size_t used = 0U;
    if (appendf(response, capacity, &used,
                "OK command=PROC\n") != 0) {
        (void)fclose(file);
        return 1;
    }

    char line[512];
    size_t fields = 0U;

    while (fgets(line, sizeof line, file) != NULL) {
        if (strncmp(line, "Name:", 5U) == 0 ||
            strncmp(line, "State:", 6U) == 0 ||
            strncmp(line, "Threads:", 8U) == 0 ||
            strncmp(line, "VmRSS:", 6U) == 0) {
            if (appendf(response, capacity, &used, "%s", line) != 0) {
                (void)fclose(file);
                return 1;
            }
            ++fields;
        }
    }

    (void)fclose(file);

    if (appendf(response, capacity, &used,
                "reported_fields=%zu\n", fields) != 0) {
        return 1;
    }

    return 0;
}

static const char *elf_type(uint16_t type)
{
    switch (type) {
    case ET_REL: return "REL";
    case ET_EXEC: return "EXEC";
    case ET_DYN: return "DYN";
    case ET_CORE: return "CORE";
    default: return "OTHER";
    }
}

static int handle_elf(const char *path,
                      char *response,
                      size_t capacity)
{
    if (path == NULL || *path == '\0') {
        (void)snprintf(response, capacity,
                       "ERR code=EINVAL message=missing-path\n");
        return 1;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        (void)snprintf(response, capacity,
                       "ERR code=%d message=%s\n",
                       errno, strerror(errno));
        return 1;
    }

    Elf64_Ehdr header;
    size_t done = 0U;

    while (done < sizeof header) {
        ssize_t n = read(fd,
                         (unsigned char *)&header + done,
                         sizeof header - done);
        if (n <= 0) {
            (void)close(fd);
            (void)snprintf(response, capacity,
                           "ERR code=EINVAL message=truncated-elf\n");
            return 1;
        }
        done += (size_t)n;
    }

    (void)close(fd);

    if (header.e_ident[EI_MAG0] != ELFMAG0 ||
        header.e_ident[EI_MAG1] != ELFMAG1 ||
        header.e_ident[EI_MAG2] != ELFMAG2 ||
        header.e_ident[EI_MAG3] != ELFMAG3 ||
        header.e_ident[EI_CLASS] != ELFCLASS64 ||
        header.e_ident[EI_DATA] != ELFDATA2LSB) {
        (void)snprintf(response, capacity,
                       "ERR code=EINVAL message=unsupported-elf\n");
        return 1;
    }

    (void)snprintf(response, capacity,
                   "OK command=ELF\n"
                   "path=%s\n"
                   "class=ELF64\n"
                   "endianness=little\n"
                   "type=%s\n"
                   "machine=%u\n"
                   "entry=0x%" PRIx64 "\n"
                   "program_headers=%u\n"
                   "section_headers=%u\n",
                   path,
                   elf_type(header.e_type),
                   (unsigned)header.e_machine,
                   (uint64_t)header.e_entry,
                   (unsigned)header.e_phnum,
                   (unsigned)header.e_shnum);
    return 0;
}

int sysdiag_handle_request(const char *request,
                           char *response,
                           size_t capacity)
{
    if (request == NULL || response == NULL || capacity == 0U) {
        return 1;
    }

    if (strcmp(request, "PING") == 0) {
        (void)snprintf(response, capacity,
                       "OK command=PING\npong=yes\n");
        return 0;
    }

    if (strncmp(request, "STAT ", 5U) == 0) {
        return handle_stat(request + 5U, response, capacity);
    }

    if (strncmp(request, "PROC ", 5U) == 0) {
        return handle_proc(request + 5U, response, capacity);
    }

    if (strcmp(request, "PROC") == 0) {
        return handle_proc("self", response, capacity);
    }

    if (strncmp(request, "ELF ", 4U) == 0) {
        return handle_elf(request + 4U, response, capacity);
    }

    (void)snprintf(response, capacity,
                   "ERR code=EINVAL message=unknown-command\n");
    return 1;
}
