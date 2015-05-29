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

struct execargs_t *createExec() {
	struct execargs_t* newExec = (struct execargs_t*)malloc(sizeof(struct execargs_t));
	return newExec;
}

int exec(struct execargs_t* args) {
    return spawn(args->file, args->argv);
}


int runpiped(struct execargs_t **programs, size_t n) {
	int current_input = STDIN_FILENO;
	int current_output = STDOUT_FILENO;
	int next_input = STDIN_FILENO;
	size_t i;
	for (i = 0; i < n; i++) {
		int pipefd[2];
		if (pipe(pipefd) == -1) {	
			fprintf(stderr, "pipe failed\n");
			kill(0, SIGINT);
			return -1;
		}
	    pid_t pid = fork();
		if (pid == -1) {
			fprintf(stderr, "can't create child\n");
			kill(0, SIGINT);
			return -1;
		}
		if (pid == 0) {	//child
			close(pipefd[0]);
			if (i < n - 1) current_output = dup2(pipefd[1], STDOUT_FILENO);
//			fprintf(stderr, "out = %d\n", pipefd[1]);
//			fprintf(stderr, "in = %d\n", next_input);
			current_input = dup2(next_input, STDIN_FILENO);
			if (next_input != STDIN_FILENO) close(next_input);
			if (current_input == -1) {
				fprintf(stderr, "dup2 input failed\n");
				exit(EXIT_FAILURE);
			}
			if (current_output == -1) {
				fprintf(stderr, "dup2 output failed\n");
				exit(EXIT_FAILURE);
			}
			if (exec(*(programs + i)) == 0) {
				exit(EXIT_SUCCESS);
			} else {
				exit(EXIT_FAILURE);
			}
	    } else {	//parent
			close(pipefd[1]);
			if (next_input != STDIN_FILENO) close(next_input);
			next_input = pipefd[0];
			int status;
			pid_t w = waitpid(pid, &status, 0);
	        if (w == -1) {
				return -1;
			}
			if (!WIFEXITED(status)) {
				return -1;
	        }
		}
	}
    return 0;
}



