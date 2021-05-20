/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：server.h
*   创 建 者：YQQ
*   创建日期：2021年05月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#ifndef _SERVER_H
#define _SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>

#include<sys/socket.h>
#include<arpa/inet.h>

int server_init(int port);
int wait_for_client(int sockfd);
void menu(int connfd);
void list(int connfd);
void download(char *filename, int connfd);
void upload(char *filename, int connfd);


#endif
