#ifndef BUFIO_H
#define BUFIO_H


#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <netdb.h>
#include <sys/un.h>
#include <netinet/in.h>

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

#endif // BUFIO_H
