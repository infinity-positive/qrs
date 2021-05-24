/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：client.c
 *   创 建 者：YQQ
 *   创建日期：2021年05月20日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/

#include "client.h"

int client_init(char *ip, int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(port);
	addr.sin_addr.s_addr	= inet_addr(ip);

	if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		return -1;
	}
	printf("client init succeed\n");

	return sockfd;
}

void download(char *filename, int sockfd)
{
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
	if(fd < 0)
	{
		perror("open");
		return ;
	}
	char buf[MAXSIZE] = {0};
	int ret = recv(sockfd, buf, sizeof(buf), 0);
	if(ret <= 0)
	{
		perror("recv");
		return ;
	}
	printf("recv: %s\n", buf);
	int len;
	sscanf(buf, "len: %d", &len);
	printf("len: %d", len);

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(sockfd, buf, sizeof(buf));
		if(len)
		{
			break;
		}
		write(fd, buf, ret);
		len -= ret;
	}
	close(fd);
}

void upload(char *filename, int sockfd)
{
	int fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return ;
	}
	int length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char buf[MAXSIZE] = {0};
	sprintf(buf, "len: %d", length);
	send(sockfd, buf, sizeof(buf), 0);

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(fd, buf, sizeof(buf));
		if(ret)
		{
			break;
		}
		write(sockfd, buf, ret);
	}
	close(fd);
}

void listFileName(int sockfd)
{
	char buf[MAXSIZE] = {0};
	read(sockfd, buf, sizeof(buf));
	printf("%s\n", buf);
}

void menu(int sockfd)
{
	char buf[MAXSIZE] = {0};
	for(int i = 0; i < 4; i++)
	{
		recv(sockfd, buf, sizeof(buf), 0);
		puts(buf);
		memset(buf, 0, sizeof(buf));
	}
}
