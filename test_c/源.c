#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
int sgn(int x) {
	int z;
		if (x > 0)
			z = 1;
		else if (x < 0) 
			z = -1;
		else
			z = 0;
	return z;
}
int main(){
	int x;
		scanf("%d", &x);
		printf("%d\n",sgn(x));
		return 0;
	}



