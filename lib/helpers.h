#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

ssize_t read_ (int fd, void * buffer, size_t count);
ssize_t write_ (int fd, const void * buffer, size_t count);
ssize_t read_until(int fd, void * buf, size_t count, char delimiter);
int spawn(const char * file, char * const argv[]);

#endif // HELPERS_H
