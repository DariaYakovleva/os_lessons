#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

ssize_t read_ (int fd, void *buffer, size_t count) {
    ssize_t curS = 0;
    while ((size_t)curS < count) {
        ssize_t cur = read(fd, buffer + curS, count - curS);
        if (cur == 0) {
            return curS;
        }
        if (cur == -1) {
            return cur;
        }
        curS += cur;
    }
    return curS;
}

ssize_t write_ (int fd, const void *buffer, size_t count) {
    ssize_t curS = 0;
    while ((size_t) curS < count) {
        ssize_t cur = write(fd, buffer + curS, count - curS);
        if (cur == -1) {
            return curS;
        }
        curS += cur;
    }
    return curS;
}

ssize_t read_until(int fd, void *buffer, size_t count, char delimiter) {
    ssize_t curS = 0;
    while ((size_t)curS < count) {
        ssize_t cur = read(fd, buffer + curS, count - curS);
        if (cur == 0) {
            return curS;
        }
        if (cur == -1) {
            return cur;
        }
        int i = curS;
        curS += cur;
        while (i < curS) {
            if ((*((char *)buffer + i)) == delimiter) {
                return curS;
            }
            i++;
        }
    }
    return curS;
	
}


