/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：1_tcpserver.c
*   创 建 者：YQQ
*   创建日期：2021年05月20日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include "server.h"

int main(int argc, char * argv[])
{
	if(argc < 2)
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

		while(1)
		{
			menu(connfd);
			char buf[128] = {0};
			recv(connfd, buf, sizeof(buf), 0);

			if(strstr(buf, "list") != NULL)
			{
				list(connfd);
			}
		}
		close(connfd);
		
	}
	close(listenfd);

	return 0;
}
