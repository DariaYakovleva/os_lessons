#include "bufio.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

struct buf_t *buf_new(size_t capacity) {
    struct buf_t* newBuf = (struct buf_t*)malloc(sizeof(struct buf_t));
    newBuf->buf = malloc(capacity);
    if ((int*)newBuf->buf == 0) {
        return NULL;
    }
    newBuf->capacity = capacity;
    newBuf->size = 0;
    return newBuf;
}

void buf_free(struct buf_t *buf) {
    free(buf->buf);
}

size_t buf_capacity(struct buf_t *buf) {
    return buf->capacity;
}

size_t buf_size(struct buf_t *buf) {
    return buf->size;
}

ssize_t buf_fill(int fd, struct buf_t *buf, size_t required) {
    while (buf->size < required && buf->size < buf->capacity) {
        ssize_t cur = read(fd, buf->buf + buf->size, required - buf->size);
//        printf("READ2 cur=%d size=%d req=%d\n", (int)cur, (int)buf->size, (int)required);
        if (cur == 0) {
            return buf->size;
        }
        if (cur == -1) {
            return cur;
        }
        buf->size += cur;
        required -= cur;
    }
    return buf->size;
}

ssize_t buf_flush(int fd, struct buf_t *buf, size_t required) {
    size_t cur = 0;
    size_t prevSize = buf->size;
    while (cur < required) {
        ssize_t len = write(fd, buf->buf, required - cur);
        if (len == -1) {
            return -1;
        }
        cur += len;
//        printf("\nWRITE cur=%d size=%d req=%d\n", (int)cur, (int)buf->size, (int)required);
        int pos = 0;
        while (pos < (int)(buf->size - len)) {
            *((char*)buf->buf + pos) = *((char*)buf->buf + pos + len);
            pos++;
        }
        buf->size -= len;
        if (buf->size == 0) break;
    }
    return prevSize - buf->size;
}
