#include"game.h"

void init_mine(char mine [ROWS][COLS], int rows, int cols)//初始化地雷
{
	int i = 10;
	
	while (i > 0)
	{
		int x = rand() % (ROWS - 2) + 1;
		int y = rand() % (COLS - 2) + 1;
 		if (mine[x][y] != '1')
		{
			mine[x][y] = '1';
			i--;
		}
	}
}

void printboard(char show[ROW][COL], int row, int col)
{
	for (int a = 0; a <= col; a++)
	{
		printf("%d ",a);
	}
	printf("\n");
	for (int i = 0; i < row; i++)
	{
		printf("%d ", i + 1);
		for (int j = 0; j < col; j++)
		{
			printf("%c ", show[i][j]);
		}
		printf("\n");
	}
}

void playermove(char mine[ROWS][COLS], char show[ROW][COL],int row,int col,char *status,int* Playcount)
{
	int x;
	int y;
	while(1)
	{
		printf("输入坐标：row col\n");//限定x,y范围
		scanf("%d %d",&x,&y);
		while (getchar() != '\n');
		printf("\n");
		if (show[x-1][y-1]!='*')
		{
			continue;
		}
		if (0 < x && x <= row && 0 < y && y <= col)
		{
			break;
		}

	}
	if (mine[x][y] == '1')
	{
		*status = 'l';
		mine[x][y] = 'M';
		printboard(show, ROW, COL);
		return;
	}
	else
	{
		if(mine[x-1][y-1] != '1')
			detect(mine, show, row, col, x-1, y-1,Playcount);
		if (mine[x - 1][y ] != '1')
			detect(mine, show, row, col, x-1, y, Playcount);
		if (mine[x - 1][y + 1] != '1')
			detect(mine, show, row, col, x-1, y+1, Playcount);
		if (mine[x  ][y - 1] != '1')
			detect(mine, show, row, col, x, y-1, Playcount);
		detect(mine, show, row, col, x, y, Playcount);
		if (mine[x  ][y + 1] != '1')
			detect(mine, show, row, col, x, y+1, Playcount);
		if (mine[x + 1][y - 1] != '1')
			detect(mine, show, row, col, x+1, y-1,Playcount);
		if (mine[x + 1][y ] != '1')
			detect(mine, show, row, col, x+1, y, Playcount);
		if (mine[x + 1][y + 1] != '1')
			detect(mine, show, row, col, x+1, y+1, Playcount);
	}
}

int detect(char mine[ROWS][COLS], char show[ROW][COL], int row, int col, int x, int y,int *Playcount)
{
	if (x < 1 || y < 1 || y > col || x > row )
		return 0;
	if (show[x - 1][y - 1] != '*')
		return 0;
	int mines = number(mine, show, ROW, COL, x, y);
	show[x - 1][y - 1] = '0' + mines;
	(*Playcount)++;
	return 1;
}

int number(char mine[ROWS][COLS], char show[ROW][COL], int row, int col,int x,int y)
{
	int mines = 0;
	if (mine[x - 1][y - 1] == '1')
		mines++;
	if (mine[x - 1][y] == '1')
		mines++;
	if (mine[x - 1][y + 1] == '1')
		mines++;
	if (mine[x ][y - 1] == '1')
		mines++;
	if (mine[x ][y + 1] == '1')
		mines++;
	if (mine[x + 1][y - 1] == '1')
		mines++;
	if (mine[x + 1][y ] == '1')
		mines++;
	if (mine[x + 1][y + 1] == '1')
		mines++;

	return mines;
}


int is_win(char mine[ROWS][COLS], char show[ROW][COL],char *status, int* Playcount)
{
	if (*status=='l')
	{
		printf("You Lose!");
		return 0;
	}
	if (*Playcount == (ROW*COL)-10)
		*status = 'w';
	if (*status=='w')
	{
		printf("|YOU WIN!|");
		return 0;
	}
	return 1;
}


