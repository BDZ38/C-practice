#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>

int compare(int x, int y) {
	if (x > y) {
		return x;
	}
	else {
		return y;
	}
}

int main(){
	int x, y;
	scanf("%d%d", &x, &y);
	
	printf("%d\n", compare(x, y));
		return 0;
}



