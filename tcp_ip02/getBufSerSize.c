/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：bufsize.c
*   创 建 者：YQQ
*   创建日期：2021年05月21日
*   描    述：
*
*   更新日志：
*
================================================================*/

/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*
*   文件名称：tcp_ser.c
*   创 建 者：杨航
*   创建日期：2021年05月15日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket error");
		return -1;
	}

	struct sockaddr_in seraddr, cliaddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(8888);  //字节序转换函数
	seraddr.sin_addr.s_addr = inet_addr("0.0.0.0"); //地址转换函数

	/*设置端口复用*/
	int val = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
	{
		perror("setsockopt");
		return -1;
	}
	if(-1 == bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr)))
	{
		perror("bind");
		return -1;
	}

	if(listen(sockfd, 5) == -1)
	{
		printf("listen error\n");
		return -1;
	}
	printf("listen-------\n");

	int recv_buf = 0;
	socklen_t recv_len = sizeof(recv_buf);
	if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recv_buf, &recv_len) == -1)
	{
		perror("getsockopt");
		return -1;
	}
	printf("recv = %dkb\n", recv_len);

	if(getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &recv_buf, &recv_len) == -1)
	{
		perror("getsockopt");
		return -1;
	}
	printf("send = %dkb\n", recv_len);

	close(sockfd);

	return 0;
}

