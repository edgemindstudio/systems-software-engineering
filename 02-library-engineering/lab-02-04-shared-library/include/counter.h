#ifndef COUNTER_H
#define COUNTER_H

#include <stddef.h>

size_t counter_count_char(const char *text, char target);
size_t counter_count_lines(const char *text);
const char *counter_version(void);

#endif
