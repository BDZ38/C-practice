#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
int sgn(int x) {
		if (x > 0)
			x = 1;
		else if (x < 0) 
			x = -1;
		else
			x = 0;
	return x;
}
int main(){
	int x;
		scanf("%d", &x);
		printf("%d\n",sgn(x));
		return 0;
	}



