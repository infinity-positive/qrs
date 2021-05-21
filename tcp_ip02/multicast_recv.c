/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：broadcast_recv.c
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
#include<netinet/ip.h>
#include<arpa/inet.h>

#define N 64
typedef struct sockaddr SA;

int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		printf("need two arguements\n");
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("socket");
		exit(-1);
	}

	char buf[N] = {0};
	struct ip_mreq mreq;
	struct sockaddr_in myaddr, peeraddr;
	socklen_t peerlen = sizeof(peeraddr);

	//加入多播组，允许数据链路层处理指定组播包
	
	bzero(&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr		= inet_addr(argv[1]);
	mreq.imr_interface.s_addr		= htonl(INADDR_ANY);
	if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,  &mreq, sizeof(mreq)) < 0)
	{
		perror("fail to setsockopt");
		exit(-1);
	}

	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(atoi(argv[2]));

	//为了接收广播包， 接收方通常需要绑定广播地址
	myaddr.sin_addr.s_addr = inet_addr(argv[1]);
	//为套接字绑定组播地址和端口
	if(bind(sockfd, (SA *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("fail to bind");
		exit(-1);
	}

	while(1)
	{
		recvfrom(sockfd, buf, N, 0, (SA *)&peeraddr, &peerlen);
		printf("[%s: %d] %s\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), buf);
	}

	close(sockfd);

	return 0;
}
