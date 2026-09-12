#include"game.h"
void game()
{
	char mine[ROWS][COLS] = { "0" };//初始化地雷盘
	char show[ROW][COL];
	memset(show,'*',sizeof(show));//初始化棋盘
	init_mine(mine,ROWS,COLS);//放置地雷
	char status = 'c';//目前状态
	int Playcount = 0;//计数判断成功
	do
	{
		printboard(show, ROW, COL);//打印棋盘
		playermove(mine, show, ROW, COL,&status,&Playcount);
		is_win(mine,show,&status,&Playcount);
	} while (status == 'c');
	

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







