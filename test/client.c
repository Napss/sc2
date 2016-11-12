#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/sockutil.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void rcvfile(int sockfd, char *outputname);
void rcvlist(int sockfd);
void choosefile(int sockfd);
char * namefile();

int main(int argc, char** argv)
{

	char * outputname;
	if (argc < 3) {
		puts("Missing host and port");
		exit(0);
	}

	int sockfd = tcp_sock_init();
	tcp_sock_connect(sockfd, argv[1], atoi(argv[2]));
	rcvlist(sockfd);
	choosefile(sockfd);
	outputname = namefile();
	rcvfile(sockfd,outputname);
	close(sockfd);
}


void rcvfile(int sockfd,char *outputname){
	char buffer[BUFSIZ];
	int filefd;
	char *file_path;
	ssize_t read_return;
    filefd = open(outputname,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
	if (filefd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	do {
		read_return = read(sockfd, buffer, BUFSIZ);
		if (read_return == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (write(filefd, buffer, read_return) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		}
		while (read_return==8192);

		
	close(filefd);
	close(sockfd);
		

	}	

void rcvlist(int sockfd){
	char *list=" ";
	char buffer[BUFSIZ];
	int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
	buffer[bytes] = '\0';
	printf("%s", buffer);

}

void choosefile(int sockfd){

	char buffer[BUFSIZ];
	puts("Choose file : ");
 	fgets(buffer, sizeof(buffer), stdin);
    send(sockfd, buffer, strlen(buffer), 0);
}
char * namefile(){
	char buffer[BUFSIZ];
	char *outputpath;
	puts("Choose name of copied file : ");
 	fgets(buffer, sizeof(buffer), stdin);
	char* p = &buffer[0] ;
	return p ;
}
