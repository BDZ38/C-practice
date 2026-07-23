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
	int identify;
again:
	scanf("%s", input);
	if (strcmp(input, "pig") == 0)
	{
		identify = 1;
	}
	else
	{
		identify = 0;
		printf("再试试.\n");
		goto again;
	}
	switch (identify)
	{
		case 1:
			system("shutdown -a");
			printf("关机取消.\n");
			break;
		default:
			break;
	}
	return 0;
}
