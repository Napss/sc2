#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/sockutil.h"
#include "include/filetransfer.h"



int main(int argc, char** argv)
{
	if (argc < 3)
	{
		puts("Missing host and port");
		exit(0);
	}
	int sockfd = tcp_sock_init();
	tcp_sock_connect(sockfd, argv[1], atoi(argv[2]));
	download_client(sockfd);
	close(sockfd);
}

