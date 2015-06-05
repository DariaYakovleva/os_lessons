#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>


size_t const MAX_LEN = (1 << 12) + 1;
ssize_t lenBuf = 0;
int ok = 1;

struct execargs_t* add_program(char* str) {
	struct execargs_t* newExec = (struct execargs_t*)malloc(sizeof(struct execargs_t));
//	newExec->argv[] = (char*)malloc(MAX_LEN);
	int cnt = 0;
	ssize_t pos = 0;
	while (*(str + pos) != 0) {
		while (*(str + pos) == ' ' && *(str + pos) != 0) pos++;
		if (*(str + pos) == 0) break;
		char* param = malloc(MAX_LEN);
		char* curp = param;
		while (*(str + pos) != ' ' && *(str + pos) != 0) {
			*curp = *(str + pos);
			curp++;
			pos++;
		}
		*curp = 0;
		newExec->argv[cnt] = param;
		cnt++;
	}
	newExec->argv[cnt] = NULL;
	newExec->file = newExec->argv[0];
	return newExec;
}


struct execargs_t* programs[10];
ssize_t split_programs(char* str, ssize_t len) {
	if (len == -1) return -5;
	char* cur_prog = malloc(MAX_LEN);
	size_t cntProg = 0;
	ssize_t clen = 0;
	while (*(str + clen) != '\n') {
		char* curs = cur_prog;
		while (*(str + clen) != '|' && *(str + clen) != '\n') {
			*curs = *(str + clen);
			curs++;
			clen++;
		}
		*curs = 0;
		struct execargs_t* progr = add_program(cur_prog);
		programs[cntProg] = progr;
		cntProg++;
		if (*(str + clen) != '\n') clen++;
	}
	programs[cntProg] = NULL;
	//printf("cntProg = %d, file = %s, arg[1] = %s\n", (int)cntProg, programs[0]->file, programs[0]->argv[1]);
	return cntProg;
}

void handler(int signal) {
//	printf("INT\n");
}

int main(int argc, char *argv[]) {
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	sigaction(SIGINT, &act, 0);

	void *buf = malloc(MAX_LEN);
	ssize_t len;
	while (ok == 1) {
		write(STDOUT_FILENO, "$", 1);
		len = read_until(STDIN_FILENO, buf, MAX_LEN, '\n');
		if (len == 0) return 0;
//		printf("BUF = !%d!\n", (int)len);
		ssize_t cnt = split_programs((char*)buf,len);
		*(char*)buf = 0;
		if (cnt == -5) {
			continue;
		}
		if (runpiped(programs, cnt) == -1) {
			fprintf(stderr, "ooops..");
		}
	}
	free(buf);	
    return 0;
}

