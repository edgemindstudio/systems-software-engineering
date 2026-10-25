#ifndef SYSDIAG_PROTOCOL_H
#define SYSDIAG_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define SYSDIAG_MAX_REQUEST 2048U
#define SYSDIAG_MAX_RESPONSE 8192U

int sysdiag_read_exact(int fd, void *buffer, size_t bytes);
int sysdiag_write_all(int fd, const void *buffer, size_t bytes);

int sysdiag_read_frame(int fd,
                       unsigned char *buffer,
                       size_t capacity,
                       uint32_t *length_out);

int sysdiag_write_frame(int fd,
                        const unsigned char *buffer,
                        uint32_t length);

#endif
