/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：insertSort.c
*   创 建 者：YQQ
*   创建日期：2021年05月25日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void insertSort(int *arr, int len)
{
	puts("src: ");
	for(int k = 0; k < len; k++)
	{
		printf("%d ", arr[k]);
	}
	puts("");

	int i, j;
	for(i = 1; i < len; i++)
	{
		for(j = i; j > 0; j--)
		{
			if(arr[j] < arr[j-1])
			{
				arr[j]		^= arr[j-1];
				arr[j-1]	^= arr[j];
				arr[j]		^= arr[j-1];
			}
		}
	}

	puts("dest: ");
	for(int k = 0; k < len; k++)
	{
		printf("%d ", arr[k]);
	}
	puts("");
}

int main(int argc, char * argv[])
{
	int arr[8];
	srand(time(NULL));
	for(int i = 0; i < 8; i++)
	{
		arr[i] = rand()%20 + 1;
		printf("%d ", arr[i]);
		
	}
	puts("");
	int len = sizeof(arr)/sizeof(int);

	insertSort(arr, len);

	return 0;
}
