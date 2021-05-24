/*===============================================================
 *   Copyright (C) 2021 All rights reserved.
 *   
 *   文件名称：unix.c
 *   创 建 者：YQQ
 *   创建日期：2021年05月24日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/

#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/un.h>
#include<errno.h>

int main(int argc, char * argv[])
{
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_un seraddr;
	seraddr.sun_family = AF_UNIX;
	strcpy(seraddr.sun_path, "1.txt");

	remove("1.txt");

	if(bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
	{
		perror("bind");
		return -1;
	}

	if(-1 == listen(sockfd, 5))
	{
		perror("listen");
		return -1;
	}

	printf("listen------------\n");
	while(1)
	{
		int connfd = accept(sockfd, NULL, NULL);
		if(connfd == -1)
		{
			perror("accept");
			return -1;
		}

		printf("accept succeed\n");

		char buf[32] = {0};

		while(1)
		{
			bzero(buf, sizeof(buf));
			if(read(connfd, buf, sizeof(buf)) == 0)
			{
				printf("client exit\n");
				close(connfd);
				break;
			}
			printf("recv: %s\n", buf);

			write(connfd, "sb", 3);
		}
	}

	close(sockfd);

	return 0;
}
