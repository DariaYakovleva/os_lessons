#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argv, char* args[]) {
	write(STDOUT_FILENO, "fdsfds", 4);
	int dir = open("log", O_RDWR);
	if (dir == -1) {
		printf("dir(");
		return 0;
	}
	int pid = fork();
	if (pid < 0) {
		write(STDOUT_FILENO, "FAIL", 4);
	} else if (pid == 0) {
		int dup = dup2(dir, STDOUT_FILENO);
		printf("DUP = %d, dIR = %d\n", dup, dir);
		args[3] = NULL;
		int ex = execvp(args[1], args + sizeof(char*));
		printf("EX = %d\n", ex);
		if (ex != -1) exit(0);
	} else {
		int status;
		int ww = waitpid(pid, &status, 0);
		printf("WW = %d\n", ww);
	}

	return 0;

}
