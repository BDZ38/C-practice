#include"game.h"
void game()
{
	char mine[ROWS][COLS] = { "0" };//初始化地雷盘
	char show[ROW][COL] = { "0" };//初始化棋盘
	init_mine(mine,ROWS,COLS);//放置地雷

}


int main()
{
	srand((unsigned int)time(NULL));
	int input;
	printf("1.play\n");//菜单
	scanf("%d", &input);
	if (input == 1)
	{
		game();//启动游戏
	}
	return 0;
}







