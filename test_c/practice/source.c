#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
int binary_search(int arr[], int sz, int k)
{
	int left = 0;
	int right = sz - 1;
	while (1)
	{
		int mid = left + (right - left) / 2;
		if (arr[mid] < k)
			left = mid + 1;
		else if (arr[mid] > k)
			right = mid - 1;
		else
			return mid;
	}
	return -1;
}

int main()
{
	int arr[] = { 1,2,3,4,5,6,7,8,9,10 };
	int k = 7;
	int sz = sizeof(arr) / sizeof(arr[0]);

	int ret = binary_search(arr,sz,k);
	if (ret == -1)
	{
		printf("cannot find\n");
	}
	else
	{
		printf("%d\n", arr[ret]);
	}
	return 0;
}
