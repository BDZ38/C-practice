#include"game.h"

void init_mine(char mine [ROWS][COLS], int rows, int cols)//初始化地雷
{
	int i = 10;
	int x = rand();
	int y = rand();
	whlie (i > 0)
	{
		if (mine[x][y] != '1')
		{
			char mine[x][y] = '1';
			i--;
		}
	}
}

void printboard(char show[ROW][COL], int row, int col)
{

}





