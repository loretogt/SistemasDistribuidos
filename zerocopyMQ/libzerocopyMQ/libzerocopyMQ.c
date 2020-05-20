#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1024

int createMQ(const char *cola) {
    int s, leido;
	struct sockaddr_in dir;
	struct hostent *host_info;
	char buf[TAM];

	host_info=gethostbyname(getenv("BROKER_HOST"));

	dir.sin_addr=*(struct in_addr *)host_info->h_addr;
    int c= atoi(getenv("BROKER_PORT"));
	dir.sin_port=htons(c);
	dir.sin_family=PF_INET;

	while ((leido=read(0, buf, TAM))>0) {
		if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
			perror("error creando socket");
			return 1;
		}
		if (connect(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
			perror("error en connect");
			close(s);
			return 1;
		}
                if (write(s, buf, leido)<0) {
                        perror("error en write");
                        close(s);
                        return 1;
                }
                if ((leido=read(s, buf, TAM))<0) {
                        perror("error en read");
                        close(s);
                        return 1;
                }
		write(1, buf, leido);
		close(s);
        }
    return 0;
}
int destroyMQ(const char *cola){
    return 0;
}
int put(const char *cola, const void *mensaje, uint32_t tam) {
    return 0;
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
    return 0;
}
