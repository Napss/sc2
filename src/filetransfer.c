/**
 * @file	filetransfer.c
 * @author	EGELE Guillaume
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sockutil.h"
#include "filetransfer.h"

//client side functions 

void download_client(int sockfd)
{
	char * outputname;
	rcv_list(sockfd);
	choose_file(sockfd);
	outputname = name_file();
	rcv_file(sockfd,outputname);
}

void rcv_file(int sockfd,char *outputname)
{
	char buffer[BUFSIZ];
	int filefd;
	char *file_path;
	ssize_t read_return;
	filefd = open(outputname,O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
	if (filefd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	do
	{
		read_return = read(sockfd, buffer, BUFSIZ);
		if (read_return == -1) 
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (write(filefd, buffer, read_return) == -1) 
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	while (read_return==8192);	
	close(filefd);
	close(sockfd);
}

void rcv_list(int sockfd)
{
	char *list=" ";
	char buffer[BUFSIZ];
	int bytes = recv(sockfd, buffer, sizeof(buffer), 0);
	buffer[bytes] = '\0';
	printf("%s", buffer);
}

void choose_file(int sockfd)
{
	char buffer[BUFSIZ];
	puts("Choose file : ");
	fgets(buffer, sizeof(buffer), stdin);
	send(sockfd, buffer, strlen(buffer), 0);
}


char * name_file()
{
	char buffer[BUFSIZ];
	char *outputpath;
	puts("Choose name of copied file : ");
	fgets(buffer, sizeof(buffer), stdin);
	char* p = &buffer[0] ;
	return p ;
}

//server side functions 

void download_server(int cli_sockfd)
{
	char * path;
	list_files(cli_sockfd,".");
	path = get_choice(cli_sockfd);
	printf("%s\n", path);
	send_file(cli_sockfd,path);
}

void send_file(int sockfd,char *file_path)
{
	char buffer[BUFSIZ];
	int filefd;
	ssize_t read_return;
	filefd = open(file_path, O_RDWR);
	while (1) 
	{
		read_return = read(filefd, buffer, BUFSIZ);
		//puts("send");
		if (read_return == 0)
		{
			break;
		}
		if (read_return == -1) 
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (write(sockfd, buffer, read_return) == -1) 
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	puts("Finish");
}

void list_files(int sockfd,char *file_path)
{
	char buffer[BUFSIZ];
	DIR *d;
	struct dirent *dir;
	char *list = "Fichiers  : ";
	strcpy(buffer, list);
	d = opendir(file_path);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			strcat(buffer,dir->d_name);
			strcat(buffer," ");
		}
		strcat(buffer,"\n");
		closedir(d);
	}
	send(sockfd, buffer, strlen(buffer), 0);
}

char * get_choice(int cli_sockfd)
{
	char buffer[BUFSIZ];
	char *choice;
	int bytes = recv(cli_sockfd, buffer, sizeof(buffer), 0);
	printf("%i\n", bytes);
	buffer[bytes-1] = '\0';
	char* p = &buffer[0] ;
	return p ;
}
