#define _CRT_SECURE_NO_WARNINGS
#include "source.h"

int count(int stairs)
{
	if (stairs <= 2)
	{
		return stairs;
	}

	return count(stairs - 1)+count(stairs - 2);
}

int main()
{	
	int stairs;
	scanf("%d\n", stairs);
	printf(count(stairs));
	return 0;
}



