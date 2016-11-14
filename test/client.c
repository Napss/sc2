#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "include/sockutil.h"

void sendfile(int sockfd,char *path);
void listfiles(int sockfd,char *path);
char * getchoice(int cli_sockfd);

int main(int argc, char** argv)
{
	if (argc < 2) {
		puts("Missing port");
		exit(0);
	}

	int sockfd = tcp_sock_serv(atoi(argv[1]));

	puts("Server is listening to incoming connections");
	int cli_sockfd, pid;

	/* Support multiple connections  */
	char * path;

	while (1) {
		cli_sockfd = tcp_sock_accept(sockfd);
		pid = fork();
		if (pid < 0)
			error("Can not fork");
		else if (pid == 0){		

			listfiles(cli_sockfd,".");
			path = getchoice(cli_sockfd);
			printf("%s\n", path);
			sendfile(cli_sockfd,path);
			puts("Closing client's socket");
			close(cli_sockfd);
		} else {
			printf("Spawned a process for new connection: %d\n", pid);
			continue;
		}
	}

	puts("Closing server's socket");
	close(sockfd);
}


void sendfile(int sockfd,char *file_path){
	char buffer[BUFSIZ];
	int filefd;
	ssize_t read_return;
    filefd = open(file_path, O_RDWR);
	while (1) {
		read_return = read(filefd, buffer, BUFSIZ);
		//puts("send");
		if (read_return == 0)
			break;
		if (read_return == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (write(sockfd, buffer, read_return) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	puts("Finish");
}

void listfiles(int sockfd,char *file_path){
	char buffer[BUFSIZ];
    DIR *d;
    struct dirent *dir;
    char *list = "Fichiers  : ";
	strcpy(buffer, list);
    d = opendir(file_path);
    if (d){
        while ((dir = readdir(d)) != NULL){
           strcat(buffer,dir->d_name);
           strcat(buffer," ");

        }
       	strcat(buffer,"\n");

        closedir(d);
    }
    send(sockfd, buffer, strlen(buffer), 0);
}

char * getchoice(int cli_sockfd){
	char buffer[BUFSIZ];
	char *choice;
	int bytes = recv(cli_sockfd, buffer, sizeof(buffer), 0);
	printf("%i\n", bytes);
	buffer[bytes-1] = '\0';
	char* p = &buffer[0] ;
	return p ;
}
