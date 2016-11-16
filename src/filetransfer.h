/**
 * @file	filetransfer.h
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

#ifndef TRANSFER_H
#define TRANSFER_H

//client side functions 

void rcv_file(int sockfd, char *outputname);
void rcv_list(int sockfd);
void choose_file(int sockfd);
void download_client(int sockfd);
char * name_file();

//server side functions 


void send_file(int sockfd,char *path);
void list_files(int sockfd,char *path);
char * get_choice(int cli_sockfd);
void download_server(int sockfd);

#endif	// TRANSFER_H
