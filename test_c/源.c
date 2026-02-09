#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>

int main(){
	int x,y;
		scanf("%d", &x);
		if (x<0)
			y = -1;
		else if (x>0)
			y = 1;
		else
			y = 0;
		printf("%d\n", y);
		return 0;
	}



