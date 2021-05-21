/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：broadcast_send.c
*   创 建 者：YQQ
*   创建日期：2021年05月21日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include<stdio.h>

#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define N 64

typedef struct sockaddr SA;

int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		printf("need three arguements\n");
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("socket");
		return -1;
	}

	char buf[N] = "This is a multicast package\n";

	struct sockaddr_in dstaddr;
	bzero(&dstaddr, sizeof(dstaddr));
	dstaddr.sin_family		= AF_INET;
	dstaddr.sin_port		= htons(atoi(argv[2]));
	dstaddr.sin_addr.s_addr	= inet_addr(argv[1]);


	while(1)
	{
		sendto(sockfd, buf, N, 0, (SA *)&dstaddr, sizeof(dstaddr));
		sleep(1);
	}
	close(sockfd);

	return 0;
}
