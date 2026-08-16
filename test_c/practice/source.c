#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "add.h"
int stringlength(char* str)
{
	int count = 0;
	while (*str != '\0')
	{
		count++;
		str++;
	}

	return count;
}

int main()
{
	char arr[] = "abc";
	printf ("%d\n",stringlength(arr));
	return 0;
}



