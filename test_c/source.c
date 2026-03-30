#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
int main() {
	char arr[] =  "Welcome to bit !!!" ;
	char arr2[] = "##################";
	int left = 0;
	int right = strlen(arr)-1;
	
	for (left = 0; left <= 9; left++)
	{
		arr2[left] = arr[left];
		arr2[strlen(arr) - left] = arr[strlen(arr) - left];
		printf("%s\n", arr2);
		Sleep(500);
		system("cls");
	}





		return 0;
	}



