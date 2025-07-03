#include<iostream>
#include <stdlib.h> 
#include <windows.h>
using namespace std;
 
int list[8][8] = { 0 };   //棋盘状态,1是黑子，2是白子，0是空的
int p=1;	//现在该谁下，1是黑方，2是白方
const int dr[8] = { 0, 0, 1, 1, 1, -1, -1, -1 },dc[8] = { 1, -1, 0, 1, -1, 0, 1, -1 };//8个方向向量
 
bool playOK(int r, int c, int dr, int dc)					//判断某个格子的某个方向能否下子
{
	if (list[r][c] != 0)return false;
	int tr = r, tc = c;                  //tr和tc分别表示该点通过行和列往特定方向移动后的坐标
	while (tr + dr >= 0 && tr + dr < 8 && tc + dc >= 0 && tc + dc < 8 && list[tr + dr][tc + dc] == 3 - p)
	{		//循环遍历，未到达边界或者右边的棋子是对方的则循环继续，否则循环退出
		tr += dr, tc += dc;    //移动坐标
	}
	//若使循环退出的那一格里，是对方的棋子，则(r,c)可落子，否则不可落子
	if (tr == r && tc == c)return false;		//难点，这一句不可少
	if (tr + dr >= 0 && tr + dr < 8 && tc + dc >= 0 && tc + dc < 8 && list[tr + dr][tc + dc] == p)return true;
	return false;
}
 
bool OK(int r, int c)					//判断某个格子能否下子
{
	if (list[r][c])return false;
	for (int i = 0; i < 8; i++)  //只要一个方向满足可以下的条件，就可以下
		if (playOK(r, c, dr[i], dc[i]))return true;   //调用
	return false;
}
 
int num(int k)  //统计棋子数目，1是黑子，2是白子
{
	int s = 0;
	for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)if (list[i][j] == k)s++;
	return s;
}
 
void display()  //显示棋盘和棋子
{
	system("cls");
	for (int i = 0; i < 8; i++)
	{
		if (!i)
		{
			cout << "    ";
			for (int j = 0; j < 8; j++)cout << char('A' + j) << "    ";
			cout << endl;
		}
		cout << i + 1 << "  ";
		for (int j = 0; j < 8; j++)
		{			
			if (list[i][j] == 2)cout << "○";else if (list[i][j] == 1)cout << "●";
			else if (OK(i, j))cout <<"？";else	cout << " .";    //调用
			cout << "   ";
		}
		cout << endl<<endl;
	}	
	cout << "黑方：" << num(1) << "      白方：" << num(2)<<"       轮到";  //调用
	if (p == 1)cout << "黑方下\n"; else cout << "白方下\n";
	cout << "候选项:";
	for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)if (OK(i, j))    //调用
		cout << "  " << char('1' + i) << char('A' + j);
}
 
void init()  //初始化
{
	list[3][3] = list[4][4] = 2;
	list[3][4] = list[4][3] = 1;
	display();  //调用
}
 
bool end_()  //判断游戏是否结束
{
	for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)if (OK(i, j))return false;   //调用
	p = 3 - p;			//改变p的2个地方之一
	display();         //难点，这一句不可少     //调用
	for (int i = 0; i < 8; i++)for (int j = 0; j < 8; j++)if (OK(i, j))return false;  //调用
	cout << "\n游戏结束\n";
	if (num(1) < num(2))cout << "白方胜利";
	else if (num(1)>num(2))cout << "黑方胜利";
	else cout << "平局";
	return true;
}
 
void turn(int tr, int tc, int dr, int dc)  //吃子函数play()的一个方向
{
	if (!playOK(tr, tc, dr, dc))return;  //难点，这一句不可少     //调用
	while (tr + dr >= 0 && tr + dr < 8 && tc + dc >= 0 && tc + dc < 8 && list[tr + dr][tc + dc] == 3 - p)
	{
		list[tr + dr][tc + dc] = p; //在该处换掉棋子的颜色
		tr += dr, tc += dc;
	}
}
 
void play()		//落下一个子
{
	cout << "\n输入行（1-8）和列（A-H）" << endl;
	char x, y;
	cin >> x >> y;
	int r = x - '1', c = y - 'a';
	if (y>'A' && y < 'Z')c = y - 'A';
	if (!OK(r, c))  //调用
	{
		cout << "ERROR!";
		return;		
	}
	for (int i = 0; i < 8; i++)turn(r, c, dr[i], dc[i]);
	list[r][c] = p;
	p = 3 - p;		//改变p的2个地方之一
	display();			//调用
}
 
int main()
{
	SetConsoleOutputCP(65001);
	system("color f0");//白底黑字
	init();
	while (!end_())play();
	system("pause>nul");
	return 0;
}