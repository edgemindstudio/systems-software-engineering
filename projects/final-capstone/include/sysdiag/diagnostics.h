#ifndef SYSDIAG_DIAGNOSTICS_H
#define SYSDIAG_DIAGNOSTICS_H

#include <stddef.h>

int sysdiag_handle_request(const char *request,
                           char *response,
                           size_t capacity);

#endif
