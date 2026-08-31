#define _CRT_SECURE_NO_WARNINGS 1
#include "game.h"

void menu()
{
	printf("1.play   0.exit\n");
}
void game()
{
	char ret = '0';
	char board[ROW][COL] = {0};
	InitBoard(board,ROW,COL);
	DisplayBoard(board,ROW,COL);
	while (1)
	{
		PlayerMove(board,ROW,COL);
		DisplayBoard(board,ROW,COL);
		ret = Iswin(board,ROW,COL);
		if (ret != 'c')
			break;
		ComputerMove(board,ROW,COL);
		DisplayBoard(board,ROW,COL);
		ret = Iswin(board,ROW,COL);
		if (ret != 'c')
			break;
	}
	if (ret == '*')
	{
		printf("You win\n");
	}
	else if (ret == "#")
	{
		printf("You lose\n");
	}
	else if(ret == 'q')
	{
		printf("Tie\n");
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	
	int choose;
	do
	{
		menu();
		if (scanf("%d", &choose) != 1)
		{
			while (getchar() != '\n');
			choose = 1;
		}
			switch (choose)
			{
			case 1:
				printf("play\n");
				game();
				break;
			case 0:
				printf("exit\n");
				break;
			default:
				break;
			}

	} while (choose);

	return 0;
}
