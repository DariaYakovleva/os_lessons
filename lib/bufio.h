#ifndef BUFIO_H
#define BUFIO_H

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

struct buf_t {
    void* buf;
    size_t capacity;
    size_t size;
};

struct buf_t *buf_new(size_t capacity);
void buf_free(struct buf_t *buf);
size_t buf_capacity(struct buf_t *buf);
size_t buf_size(struct buf_t *buf);
ssize_t buf_fill(int fd, struct buf_t *buf, size_t required);
ssize_t buf_flush(int fd, struct buf_t *buf, size_t required);
ssize_t buf_getline(int fd, struct buf_t *buf, char* dest);
ssize_t buf_write(int fd, struct buf_t *buf, char* src, size_t len);

#endif // BUFIO_H
