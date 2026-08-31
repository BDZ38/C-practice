#define _CRT_SECURE_NO_WARNINGS 1
#include"game.h"

void InitBoard(char board[ROW][COL], int row, int col)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			board[i][j] = ' ';

		}
	}
}

void DisplayBoard(char board[ROW][COL], int row, int col)
{
	//打印行
	int a;
	for (a = 0; a < row;a++)
	{
		
		for (int b = 0; b < col; b++)
		{
			printf(" %c |", board[a][b]);
		}
		printf("\n");
		
		for (int c = 0; c < col; c++)
		{
			printf("----");
		}
		printf("\n");


	}
}

void PlayerMove(char board[ROW][COL], int row, int col)
{
	int x = 0;
	int y = 0;
	printf("Player's turn\n");

	while (1)
	{
		printf("Input row col (1-%d 1-%d)\n", row, col);
		if (scanf("%d %d", &x, &y) != 2)
		{
			while (getchar() != '\n');
			printf("Invalid input\n");
			continue;
		}
		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (board[x - 1][y - 1] == ' ')
			{
				board[x - 1][y - 1] = '*';
				break;
			}
			else
			{
				printf("coordinate occupied\n");
			}
		}
	}
}

void ComputerMove(char board[ROW][COL], int row, int col)
{
	printf("Computer's turn\n");
	int x = 0;
	int y = 0;

	while (1)
	{
		x = rand() % row;
		y = rand() % col;
		if (board[x][y] == ' ')
		{
			board[x][y] = '#';
			break;
		}
	}

}

char Iswin(char board[ROW][COL], int row, int col)
{
	int i;
	for (i = 0; i < row; i++)
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
		{
			return board[i][0];
		}
	}
	int j;
	for (j = 0; j < col; j++)
	{
		if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != ' ')
		{
			return board[0][j];
		}
	}
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
	{
		return board[0][0];
	}
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')
	{
		return board[1][1];
	}

	for (int m = 0; m < row; m++)
	{
		for (int n = 0; n < col; n++)
		{
			if (board[m][n] == ' ')
			{
				return 'c';
			}
		}
	}
	return 'q';
}

