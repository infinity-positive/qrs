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
			char buf1[32] = {0};
			int ret = recv(connfd, buf, sizeof(buf), 0);
			if(ret <= 0)
			{
				printf("client quit\n");
				close(connfd);
				break;
			}

			printf("recv: %s\n", buf);
			if(strstr(buf, "list") != NULL)
			{
				list(connfd);
			}
			else if(strstr(buf, "download") != NULL)
			{
				memset(buf1, 0, sizeof(buf1));
				sscanf(buf, "download %s", buf1);
				download(buf1, connfd);
				printf("download succeed\n");
			}
			else if(strstr(buf, "upload") != NULL)
			{
				memset(buf1, 0, sizeof(buf1));
				sscanf(buf, "upload %s", buf1);
				upload(buf1, connfd);
				printf("upload succeed\n");
			}
			else if(strstr(buf, "quit") != NULL)
			{
				printf("client exit\n");
				break;
			}
			else
			{
				memset(buf, 0, sizeof(buf));
				strcpy(buf, "please input according to menu\n");
				send(connfd, buf, strlen(buf), 0);
			}
		}
		close(connfd);

	}
	close(listenfd);

	return 0;
}
