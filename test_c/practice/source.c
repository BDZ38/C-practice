#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//判断素数
int is_prime (int x)
{
	int j;
	for (j = 2; j <= sqrt(x); j++)
	{
		if (0 == x % j)
		{
			return 0;
		}
	}
		return 1;
}

int main()
{
	int i;
	for (i = 101; i <= 200; i+=2)
	{
		if (is_prime(i))
		printf("%d\n",i);
	}
	return 0;
}
