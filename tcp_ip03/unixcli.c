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

	if(connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) == -1)
	{
		perror("connect");
		return -1;
	}

	char buf[32] = {0};
	while(1)
	{
		bzero(buf, sizeof(buf));
		printf("input string :");
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf)-1] = '\0';
		write(sockfd, buf, strlen(buf));
		
		bzero(buf, sizeof(buf));
		read(sockfd, buf, sizeof(buf));
		printf("recv: %s\n", buf);

	}

	close(sockfd);

	return 0;
}
