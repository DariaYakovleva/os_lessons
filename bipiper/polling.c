#include "../lib/bufio.h"
#include <sys/stat.h>
#include <sys/poll.h>

const int N = 127;
const int TIMER = 1;
int work = 1;

int createServer(char* port) {
	int server;
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	if  ((server = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
		fprintf(stderr, "socket error\n");
		exit(EXIT_FAILURE);
	}
	if (bind(server, servinfo->ai_addr, servinfo->ai_addrlen) == -1)	 {
		fprintf(stderr, "can't bind port %s\n", port);
		close(server);
		exit(EXIT_FAILURE);
	}
	if (listen(server, 10) == -1) {
		fprintf(stderr, "listen error\n");
		close(server);
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(servinfo);
	return server;
}

typedef struct {
	struct buf_t* buf1;
	struct buf_t* buf2;
} buf_pair;

struct pollfd fds[256];
buf_pair buffs[127];
int cnt_fds = 0;


int add_clients() {
	struct sockaddr_storage client_addr;
	socklen_t addr_size = sizeof(client_addr);

	int client1 = accept(fds[0].fd, (struct sockaddr*)&client_addr, &addr_size);
	fprintf(stderr, "cl1 = %d\n", client1);
	if (client1 == -1) {
		fprintf(stderr, "accept socket failed\n");
		return -1;
	}
	int client2 = accept(fds[1].fd, (struct sockaddr*)&client_addr, &addr_size);
	fprintf(stderr, "cl2 = %d\n", client2);
	if (client2 == -1) {
			fprintf(stderr, "accept socket failed\n");
			close(client1);
			return -1;
	}
	fds[cnt_fds * 2 + 2].fd = client1;
	fds[cnt_fds * 2 + 2].events = POLLIN | POLLOUT | POLLRDNORM;
	fds[cnt_fds * 2 + 1 + 2].fd = client2;
	fds[cnt_fds * 2 + 1 + 2].events = POLLIN | POLLOUT | POLLRDNORM;
	buffs[cnt_fds].buf1 = buf_new(4096);
	buffs[cnt_fds].buf2 = buf_new(4096);
	cnt_fds++;
	return 0;
}
// fds[0] -- server1
// fds[1] -- server2

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "I want more arguments\n");
	}
	fds[0].fd = createServer(argv[1]);
	fds[0].events = POLLIN;
	fds[1].fd = createServer(argv[2]);
	fds[1].events = POLLIN;
	while (work) {
		int pp = poll(fds, N * 2 + 2, TIMER * 1000);
		fprintf(stderr, "poll = %d\n", pp);
		if (pp == 0) {
			continue;
		}
		if (pp == -1 && errno != EINTR) {
			fprintf(stderr, "poll error %d\n", errno);
			break;
		}
		int i;
		for (i = 0; i < cnt_fds * 2 + 2; i++) {
			fprintf(stderr, "que cl = %d, event = %d\n", fds[i].fd, fds[i].revents);
			if (fds[i].revents & POLLIN) {
				if (i < 2) {
					add_clients();
					continue;
				}
			}
			if (fds[i].revents & POLLIN) {
				struct buf_t* buf;
				if (i % 2 == 0) {
					buf = buffs[i / 2 - 1].buf1;
				} else {
					buf = buffs[i / 2 - 1].buf2;
				}
				if (buf_fill(fds[i].fd, buf, 1) == -1) {
					fprintf(stderr, "socket %d is closed\n", fds[i].fd);
					break;
				}
			}
			if (fds[i].revents & POLLOUT) {
				struct buf_t* buf;
				if (i % 2 == 0) {
					buf = buffs[i / 2 - 1].buf2;
				} else {
					buf = buffs[i / 2 - 1].buf1;
				}
				if (buf_flush(fds[i].fd, buf, buf_size(buf)) == -1) {
						fprintf(stderr, "write error\n");
				}
			}
		}
	}
	close(fds[0].fd);
	close(fds[1].fd);
    return 0;
}

