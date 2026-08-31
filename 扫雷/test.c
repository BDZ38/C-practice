#include"game.h"
void game()
{
	char mine[ROWS][COLS] = { "0" };//初始化地雷盘
	char show[ROW][COL] = { "0" };//初始化棋盘
	init_mine(mine,ROWS,COLS);//放置地雷

}


int main()
{
	int input;
	printf("1.play");//菜单
	scanf("%d", &input);
	char start[] = "1";
	if (strcmp(input,start) == 0)
	{
		game();//启动游戏
	}
	return 0;
}







