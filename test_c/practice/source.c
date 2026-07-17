#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
void menu()
{
	printf("%s\n", "Menu");
	printf("%s\n", "1.play");
	printf("%s\n","0.exit");
}
void game()
{
	srand ((unsigned int)time(NULL));
	int ret = rand();
	printf("%d\n",ret);
}
int main() {
	int input = 0;
	do
	{
		menu();
		printf("%c\n", '>');
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			game();

			break;
		case 0:
			printf("%s\n","exit game");
			break;
		default:
			printf("again\n");
			break;
		}
	} while (input != 0);





}



