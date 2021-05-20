/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：4_tcpclient.c
*   创 建 者：YQQ
*   创建日期：2021年05月19日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

int client_init(char *ip, int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family				= AF_INET;
	addr.sin_port				= htons(port);
	addr.sin_addr.s_addr		= inet_addr(ip);

	if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		return -1;
	}
	printf("connect to server succeed\n");

	return sockfd;
}

int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		printf("need three arguements\n");
		return -1;
	}
	int sockfd = client_init(argv[1], atoi(argv[2]));
	if(sockfd < 0)
	{
		return -1;
	}
	char buf[256] = {0};
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		recv(sockfd, buf, sizeof(buf), 0);
		printf("%s\n", buf);
		sleep(1);
		memset(buf, 0, sizeof(buf));
		printf("input: ");
		gets(buf);
		send(sockfd, buf, strlen(buf), 0);
		
		char buf1[32] = {0}; 
		sscanf(buf, "download %s", buf1);
		memset(buf, 0, sizeof(buf));
		int ret = recv(sockfd, buf, sizeof(buf), 0);
		if(ret <= 0)
		{
			break;
		}
		printf("%s\n", buf);
		if(strstr(buf, "len: ") != NULL)
		{
			int length;
			sscanf(buf, "len: %d", &length);
			char *p = (char *)malloc(length);
			int count = 0;
			while(count < length)
			{
				int ret = recv(sockfd, p+count, length-count, 0);
				if(ret <= 0)
				{
					break;
				}
				count += ret;
			}
			int fd = open(buf1, O_WRONLY | O_CREAT | O_TRUNC, 0640);
			if(fd < 0)
			{
				perror("open");
				return -1;
			}
			write(fd, p, length);
			close(fd);
			free(p);
		}
	}

	return 0;
}
