/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：client.h
 *   创 建 者：YQQ
 *   创建日期：2021年05月20日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/
#ifndef _CLIENT_H
#define _CLIENT_H

#include<stdio.h>

#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXSIZE 128

int client_init(char *ip, int port);
void download(char *filename, int connfd);
void upload(char *filename, int connfd);
void listFileName(int sockfd);
void list(int sockfd);

#endif
