#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>

int main() {
	int n;
	scanf("%d",&n);
	int a = 1;
	int ret = 1;
	int sum = 0;
	for (a=1;a<=n;a++)
	{
		ret *= a;
		sum += ret;
	}
	printf("%d", sum);






		return 0;
	}



