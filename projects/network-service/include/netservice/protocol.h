#ifndef NETSERVICE_PROTOCOL_H
#define NETSERVICE_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define NETSERVICE_MAX_FRAME 4096U

int netservice_read_exact(int fd, void *buffer, size_t bytes);
int netservice_write_all(int fd, const void *buffer, size_t bytes);

int netservice_read_frame(int fd,
                          unsigned char *buffer,
                          size_t capacity,
                          uint32_t *length_out);

int netservice_write_frame(int fd,
                           const unsigned char *buffer,
                           uint32_t length);

#endif
