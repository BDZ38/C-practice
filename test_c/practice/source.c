#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	printf("电脑即将关机，输入pig以取消.\n");
	system("shutdown -s -t 60");
	char input[20] = { 0 };
	do
	{
		scanf("%s", input);
		printf("再试试.\n");
	}
	while(strcmp(input, "pig") != 0);
	system("shutdown -a");
	printf("关机取消.\n");
	return 0;
}
