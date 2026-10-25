#ifndef SYSUTIL_COMMON_H
#define SYSUTIL_COMMON_H

#include <stddef.h>
#include <sys/types.h>

int sysutil_error_errno(const char *operation, const char *path);
int sysutil_write_all(int fd, const void *buffer, size_t size);
const char *sysutil_file_type(mode_t mode);

#endif
