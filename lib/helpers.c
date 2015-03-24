#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

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

int spawn(const char * file, char * const argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        printf("fail\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        pid_t w = waitpid(pid, &status, 0);
        if (w == -1) {
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            exit(EXIT_FAILURE);
        }
    } else {
        if (execvp(file, argv) != -1) {
            exit(0);
        }
        exit(EXIT_FAILURE);
    }

}


