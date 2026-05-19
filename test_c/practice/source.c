#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
int main() {
	
	int login_times = 0;
	for (login_times = 0;login_times<3;login_times++)
	{
		char input[20];
		scanf("%s", input);

		if (strcmp(input,"123")==0)
		{
			printf("Success!\n");
			break;
		}
		else
		{
			printf("Password incorrect,please retry.\n");
		}
	}
	if (login_times == 3)
	{
		printf("account frozen\n");
	}


		return 0;
	}



