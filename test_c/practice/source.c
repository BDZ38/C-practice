#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>

int findmax(int x, int y)
{
	if (x > y)
		return x;
	else if (x < y)
		return y;
	else
		return x;
}
int main()
{
	int x;
	int y;
	scanf("%d %d", & x,& y);
	int z = findmax(x, y);
	printf("%d\n", z);
	return 0;
}
