#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//判断素数

int main()
{
	int i;
	for (i = 1; i <= 200; i++)
	{
		int flag =1;
		int j;
		for (j = 2; j <= sqrt(i); j++)
		{
			if (0 == i%j)
			{
				flag = 0;
				break;
			}
			
		}
		if (flag == 1)
		{
			printf("%d\n", i);
		}
	}
	return 0;
}
