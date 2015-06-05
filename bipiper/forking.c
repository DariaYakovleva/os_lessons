#include "../lib/bufio.h"
#include <sys/stat.h>
#include <fcntl.h>

size_t const MAX_LEN = 1 << 17;
char* port1;
char* port2;
int serverSocket1;
int serverSocket2;
int work = 1;
int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "I want more arguments\n");
	}
	port1 = argv[1];
	port2 = argv[2];
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo1;
	struct addrinfo *servinfo2;
	struct sockaddr_storage client_addr;
	socklen_t addr_size = sizeof(client_addr);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, port1, &hints, &servinfo1)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	if ((status = getaddrinfo(NULL, port2, &hints, &servinfo2)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	if  ((serverSocket1 = socket(servinfo1->ai_family, servinfo1->ai_socktype, servinfo1->ai_protocol)) == -1 || (serverSocket2 = socket(servinfo2->ai_family, servinfo2->ai_socktype, servinfo2->ai_protocol)) == -1) {
		fprintf(stderr, "socket error\n");
		exit(EXIT_FAILURE);
	}
	if (bind(serverSocket1, servinfo1->ai_addr, servinfo1->ai_addrlen) == -1 || bind(serverSocket2, servinfo2->ai_addr, servinfo2->ai_addrlen) == -1) {
		fprintf(stderr, "can't bind port %s or %s\n", port1, port2);
		close(serverSocket1);
		close(serverSocket2);
		exit(EXIT_FAILURE);
	}
	if (listen(serverSocket1, 10) == -1 || listen(serverSocket2, 10) == -1) {
		fprintf(stderr, "listen error\n");
		close(serverSocket1);
		close(serverSocket2);
		exit(EXIT_FAILURE);
	}
	while (work) {
		addr_size = sizeof(client_addr);
		int clientSocket1 = accept(serverSocket1, (struct sockaddr*)&client_addr, &addr_size);
//		fprintf(stderr, "cl1 = %d %d\n", clientSocket1, EAGAIN);
		if (clientSocket1 == -1) {
			fprintf(stderr, "accept socket failed\n");
			continue;
		}
		int clientSocket2 = accept(serverSocket2, (struct sockaddr*)&client_addr, &addr_size);
//		fprintf(stderr, "cl2 = %d %d\n", clientSocket2, EAGAIN);
		if (clientSocket2 == -1) {
				fprintf(stderr, "accept socket failed\n");
				continue;
		}
		int num = 0;
		for (num = 0; num < 2; num++) {
			pid_t pid = fork();
			if (pid == -1) {
				fprintf(stderr, "fork error\n");
			}
			if (pid == 0) {
				int cap = 4096;
				struct buf_t* buf = buf_new(cap);
				if (num == 0) {
					ssize_t lenR;
					size_t prev = buf_size(buf);
					while ((lenR = buf_fill(clientSocket1, buf, 1)) > 0) {
						ssize_t len;
						if ((len = buf_flush(clientSocket2, buf, lenR)) == -1) {
							fprintf(stderr, "write error\n");
							shutdown(clientSocket2, SHUT_WR);							
//							exit(EXIT_FAILURE);
						}
					}
					if (buf_size(buf) == prev) {
						shutdown(clientSocket1, SHUT_RD);
					}
				} else {
					ssize_t lenR;
					size_t prev = buf_size(buf);
					while ((lenR = buf_fill(clientSocket2, buf, 1)) > 0) {
						ssize_t len;
						if ((len = buf_flush(clientSocket1, buf, lenR)) == -1) {
							fprintf(stderr, "write error\n");
							shutdown(clientSocket1, SHUT_WR);
//							close(clientSocket1);
//							close(clientSocket2);
//							exit(EXIT_FAILURE);
						}
					}
					if (buf_size(buf) == prev) {
						shutdown(clientSocket2, SHUT_RD);
					}
				}
//				close(clientSocket1);
//				close(clientSocket2);
				exit(EXIT_SUCCESS);
			} else {
				if (num == 1) {
					close(clientSocket1);
					close(clientSocket2);
				}
			}
		}
	}
	close(serverSocket1);
	freeaddrinfo(servinfo1);
	close(serverSocket2);
	freeaddrinfo(servinfo2);
    return 0;
}

