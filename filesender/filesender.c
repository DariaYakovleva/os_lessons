#include "../lib/bufio.h"

size_t const MAX_LEN = 1 << 17;
char* port;
char* filename;
int serverSocket;
int work = 1;
int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "I want more arguments\n");
	}
	port = argv[1];
	filename = argv[2];
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct sockaddr_storage client_addr;
	socklen_t addr_size = sizeof(client_addr);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	if ((serverSocket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
		fprintf(stderr, "socket error\n");
		exit(EXIT_FAILURE);
	}
	if (bind(serverSocket, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
		fprintf(stderr, "can't bind port %s\n", port);
		exit(EXIT_FAILURE);
	}
	if (listen(serverSocket, 10) == -1) {
		fprintf(stderr, "listen error\n");
		exit(EXIT_FAILURE);
	}
	while (work) {
		addr_size = sizeof(client_addr);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&client_addr, &addr_size);
		if (clientSocket == -1) {
			fprintf(stderr, "aceept socket failed\n");
			continue;
		}
		int cap = 4096;
		struct buf_t* buf = buf_new(cap);
		buf_fill(clientSocket, buf, cap);
		if (write(clientSocket, buf->buf, buf_size(buf)) == -1) {
			fprintf(stderr, "write error\n");
		}	
		close(clientSocket);
	}
	


	close(serverSocket);
	freeaddrinfo(servinfo);
    return 0;
}

