#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
void add(int * num)
{
	*num = *num+1;
}

int main()
{
	int num = 0;
	printf("%d\n", num);
	add(&num);
	printf("%d\n", num);
	return 0;
}
