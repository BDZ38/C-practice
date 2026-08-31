#include"game.h"

void init_mine(char mine [ROWS][COLS], int rows, int cols)//³õÊ¼»¯µØÀ×
{
	int i = 10;
	int x = rand()%ROWS;
	int y = rand()%COLS;
	while (i > 0)
	{
		if (mine[x][y] != '1')
		{
			mine[x][y] = '1';
			i--;
		}
	}
}

void printboard(char show[ROW][COL], int row, int col)
{

}





