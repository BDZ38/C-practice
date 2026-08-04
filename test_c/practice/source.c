#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>

void switch_int(int *x,int *y)
{
	int z = *x;
	*x = *y;
	*y = z;
}

int main()
{
	int a;
	int b;
	scanf("%d %d", &a, &b);
	int* m = &a;
	int* n = &b;
	switch_int(m, n);
	printf("%d\n", a);
	printf("%d\n", b);
	return 0;
}
