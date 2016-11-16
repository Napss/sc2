#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/sockutil.h"
#include "include/filetransfer.h"



int main(int argc, char** argv)
{
	if (argc < 2) 
	{
		puts("Missing port");
		exit(0);
	}

	int sockfd = tcp_sock_serv(atoi(argv[1]));
	puts("Server is listening to incoming connections");
	int cli_sockfd, pid;

	/* Support multiple connections  */

	while (1) 
	{
		cli_sockfd = tcp_sock_accept(sockfd);
		pid = fork();
		if (pid < 0)
			error("Can not fork");
		else if (pid == 0)
		{		
			download_server(cli_sockfd);
			puts("Closing client's socket");
			close(cli_sockfd);
		}
		else
		{
			printf("Spawned a process for new connection: %d\n", pid);
			continue;
		}
	}

	puts("Closing server's socket");
	close(sockfd);
}
