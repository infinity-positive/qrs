/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：3_tcpserver.c
 *   创 建 者：YQQ
 *   创建日期：2021年05月19日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>

int server_init(int port)
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
	addr.sin_addr.s_addr	= inet_addr("0.0.0.0");

	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return -1;
	}

	if(listen(sockfd, 5) < 0)
	{
		perror("listen");
		return -1;
	}

	printf("server init success\n");
	return sockfd;
}

int wait_for_client(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int connfd = accept(sockfd, (struct sockaddr *)&addr, &len);
	if(connfd < 0)
	{
		perror("accept");
		return -1;
	}

	printf("client: %s:%u\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	return connfd;
}

void menu(int connfd)
{
	char buf[128] = {0};
	strcpy(buf, "---------------------------------------------------------------------\n");
	write(connfd, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	strcpy(buf, "  1.list  2.download filename  3.upload filename  4.quit\n");
	write(connfd, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	strcpy(buf, "---------------------------------------------------------------------\n");
	write(connfd, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	strcpy(buf, "cmd >>>\n");
	write(connfd, buf, strlen(buf));
}

//向客户端发送数据（客户端下载）
void download(char *filename, int connfd)
{
	int fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		send(connfd, "NO such file", 13, 0);
		return ;
	}
	int length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char buf[128] = {0};
	sprintf(buf, "len: %d", length);
	send(connfd, buf, strlen(buf), 0);

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		int ret = read(fd, buf, sizeof(buf));
		if(ret <= 0)
		{
			break;
		}
		send(connfd, buf, ret, 0);
	}
	close(fd);
}
//列出服务器目录下文件
int list(int connfd)
{
	DIR *dir = opendir("./");
	if(dir == NULL)
	{
		perror("opendir");
		return -1;
	}

	struct dirent *filestream;
	char buf[128] = {0};
	while(filestream = readdir(dir))
	{
		if((strncmp(filestream->d_name, ".", 1) == 0) || (strncmp(filestream->d_name, "..", 2) == 0))
			continue;
		memset(buf, 0, sizeof(buf));
		strcpy(buf, filestream->d_name);
		strcat(buf, "\n");
		send(connfd, buf, strlen(buf), 0);
	}
}

int main(int argc, char * argv[])
{
	if(argc < 0)
	{
		printf("need two arguements\n");
		return -1;
	}
	int listenfd = server_init(atoi(argv[1]));
	if(listenfd < 0)
	{
		return -1;
	}
	while(1)
	{
		int connfd = wait_for_client(listenfd);
		if(connfd < 0)
		{
			continue;
		}

		char buf[128] = {0};
		while(1)
		{
			menu(connfd);
			memset(buf, 0, sizeof(buf));
			int ret = recv(connfd, buf, sizeof(buf), 0);
			if(ret < 0)
			{
				perror("recv");
				break;
			}
			else if(ret == 0)
			{
				break;
			}
			else
			{
				printf("recv: %s\n", buf);
				if(strstr(buf, "list") != NULL)
				{
					list(connfd);
					sleep(1);
				}
				else if(strstr(buf, "download") != NULL)
				{
					char buf1[32] = {0};
					sscanf(buf, "download %s", buf1);
					download(buf1, connfd);
				}
				else if(strstr(buf, "upload") != NULL)
				{

				}
				else if(strstr(buf, "quit") != NULL)
				{
					break;
				}
				else
				{
					memset(buf, 0, sizeof(buf));
					strcpy(buf, "Input according to menu\n");
					send(connfd, buf, strlen(buf), 0);
				}
			}
		}
		close(connfd);
	}
	close(listenfd);

	return 0;
}
