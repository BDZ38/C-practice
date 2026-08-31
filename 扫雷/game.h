#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include<time.h>
#include <string.h>
#include<stdlib.h>
#define ROW 9	//定义矩阵行列
#define COL 9
#define ROWS (ROW+2)
#define COLS (COL+2)


void init_mine(char mine[ROWS][COLS], int rows, int cols);
void printboard(char show[ROW][COL], int row, int col);
