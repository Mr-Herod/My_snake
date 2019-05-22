//#include "pch.h"
//#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
//#include <own.h>
#include <conio.h>
#define MAXLENTH 2000

struct score {
	int current_score;
	char name[30];
	int highest[5];
}history;

struct position {
	short  x,y;
	struct position * pre,* next;
};

struct tm * lt;
typedef struct position snake;
typedef snake * point;
time_t t;
long long step = 0,pre_step = 0;
point Head, Tail, temp,te;
FILE * RankFile,*HistoryFile;

	short snake_lenth, Food[2];         //记录蛇身长度，食物坐标
	short game, a_or_m, speedx, speedy; //记录游戏状态，游戏模式，移动速度
	char  snake_dir = 'd', former_dir;  //记录蛇头方向和前一次移动的方向
	void goto_xy(SHORT, SHORT);         //移动光标到x,y指示的坐标下
	void over_game(void);               //结束游戏
	void begin_game(void);              //开始游戏
	void win_game(void);                //赢得游戏
	void draw_map(void);                //打印地图
	void draw_snake(void);              //打印蛇身
	void draw_menu(void);               //打印菜单
	void draw_food(void);               //打印食物
	void write_history();               //记录游戏历史
	void change_dir_auto(void);         //自动改变方向
	void change_dir_mannul(void);       //手动改变方向
	void move_snake();                  //移动蛇身
	short check_game();                 //检查游戏状态
	void test(void);                    //测试接口


int main(int argc, char *argv[])        //主函数开始
{
//	test();
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);//隐藏光标
	CONSOLE_CURSOR_INFO cinfo;                  //隐藏光标
	cinfo.bVisible = 0;                         //隐藏光标
	cinfo.dwSize = 1;                           //隐藏光标
	SetConsoleCursorInfo(fd, &cinfo);           //隐藏光标
	begin_game();                               //开始游戏

	while (1) {
		game = check_game();                    //判断游戏状态
		if (game == 1) {                        //吃到食物，增长蛇身
			temp = (point)malloc(sizeof(snake));//尾插法增加蛇身节点
			temp->x = Tail->x;temp->y = Tail->y;
			Tail->next = temp;
			temp->pre = Tail;
			Tail = temp;
			goto_xy(84, 8);
			history.current_score += 10;        //得分加+10
			printf("%5d",history.current_score);//更新得分
			draw_food();                        //重新生成食物
			move_snake();                       //继续移动蛇身
			snake_lenth++;                      //蛇身长度+1
		}
		else if (game == 2)
			move_snake();                       //移动蛇身
		else if (game == 0)
			over_game();                        //结束游戏
		else if (game == 3)
			win_game();                         //赢得游戏
		else if (game == 4)
		{
			system("CLS");                      //清屏
			goto_xy(33, 12);                    //移动光标
			printf("See U next time!!!");       //打印结束信息
			break;                              //退出主循环，结束程序
		}
	}
	getchar();
}

void draw_map()    //打印地图
{
	goto_xy(0, 0);
	short tmp;
	for (tmp = 1; tmp <= 78; tmp++)
		printf("=");
	printf("\n");
	for (tmp = 1; tmp <= 22; tmp++)
		printf("*                                                                            *\n");
	for (tmp = 1; tmp <= 78; tmp++)
		printf("=");
}

void draw_snake()    //打印初始蛇身
{
	goto_xy(1, 1);
	system("color 2E");
	printf("OOO@");

	temp = (point)malloc(sizeof(snake));    //生成一个蛇头节点
	temp->x = 4;temp->y = 1;
	Head = Tail = temp;
    for(int i = 3 ; i > 0 ; i --)           //生成三个蛇身节点
	{
	    temp = (point)malloc(sizeof(snake));
        temp->x = i;temp->y = 1;            //记录蛇身坐标
        temp->pre = Tail;
        Tail->next = temp;
        Tail = temp;
	}
	Tail->next = NULL;
	Head->pre = NULL;
	snake_lenth = 4;                        //蛇身长度初始化为4
	snake_dir = 'r';                        //设置蛇的初始方向为右
}

void begin_game()   //初始化游戏
{
	srand((unsigned)time(NULL));                    //更新随机种子
	draw_menu();                                    //打印地图

	RankFile=fopen("rank.txt","r");                 //打开历史记录文件
	for(int tmp=0;tmp<5;tmp++)                      //读取历史记录
        fscanf(RankFile,"%d",&(history.highest[tmp]));
	fclose(RankFile);                               //关闭历史记录文件

	history.current_score = 0;                      //初始化分数
	if (game != 4)                                  //初始化地图，蛇身，食物
	{
		system("color 1E");
		system("CLS");
		system("color 2E");
		draw_map();
		draw_snake();
		draw_food();
	}
	goto_xy(80,3);
	printf("highest score: ");                      //显示历史最高分
	goto_xy(84,4);
	printf("%5d",history.highest[0]);               //显示历史最高分
	goto_xy(80, 7);
	printf("current score: ");
	goto_xy(84, 8);
	printf("%5d", history.current_score);           //显示当前得分
}

void draw_food()    //打印食物
{
    /**
    随机生成食物坐标，判断是否在蛇身上
    如果是，则重新生成一个坐标
    否则，就打印食物
    **/
	short tmp1, tmp2;
	tmp1 = rand() % 75 + 1;
	tmp2 = rand() % 22 + 1;
	te = Tail;
	while (te->pre != NULL)     //判断新生成的坐标是否与蛇身重叠
	{
		if (tmp1 == te->x && tmp2 == te->y)
		{
			tmp1 = rand() % 76 + 1;
			tmp2 = rand() % 22 + 1;
		}
		te = te->pre;
	}
	goto_xy(tmp1, tmp2);
	Food[0] = tmp1;
	Food[1] = tmp2;
	printf("$");                //打印食物
}

void draw_menu()                //打印菜单
{
	system("CLS");
	system("color 1E");
	goto_xy(0, 0);
	printf("\
 ******************************************************************************\n\
 *                                                                            *\n\
 *                            1:Auto play                                     *\n\
 *                                                                            *\n\
 *                            2:Mannual play                                  *\n\
 *                                                                            *\n\
 *                            3:Exit                                          *\n\
 *                                                                            *\n\
 ******************************************************************************");
	a_or_m = _getch();          //读取输入
	_getch();                   //获取选项
	if (a_or_m == 51)
		game = 4;
}

short check_game()              //判断游戏状态
{
	if (game == 4)              //结束游戏
		return 4;
	else if (snake_lenth == MAXLENTH)                   //满分通关，赢得游戏
		return 3;
	else if (Head->x == 77 || Head->y == 23 || Head->x == 0 || Head->y == 0)    //撞墙死亡
		return 0;
	else if (Head->x == Food[0] && Head->y == Food[1])  //吃到食物
		return 1;
	else                                                //正常移动
		return 2;
}

void move_snake() {         //移动蛇身
	short tmp;
	if (a_or_m == 49) {     //判断是手动模式还是自动模式
		if (_kbhit() == 1)  //自动模式中有键盘输入就结束游戏
            over_game();    //结束游戏
		speedx = 1;         //设置速度
		speedy = 1;         //设置速度
		change_dir_auto();  //自动改变方向
	}
	else if (_kbhit() == 1) { //手动模式有键盘输入则读取输入，改变方向
		//speedx = 10;
		//speedy = 10;
		change_dir_mannul();  //手动改变方向
		step = 0;
	}
	else {
		speedx = 100;         //设置移动速度
		speedy = 150;         //设置移动速度
	}

	if(pre_step != 0 && step ++ == 0)
        {pre_step = 0;return;}
    pre_step = step - 1;
	te = Tail;
	tmp = 1;
	while (te->pre != NULL)   //判断蛇尾是否与蛇身重叠
	{
		if (Tail->x == te->pre->x && Tail->y == te->pre->y)
		{
			tmp = 0;
			break;
		}
		te = te->pre;
	}
	if (tmp != 0)              //没有重叠则蛇尾置为空白
	{
		goto_xy(Tail->x, Tail->y);
		printf(" ");
	}

	te=Tail;
	/**
	调整蛇身链表，去掉蛇尾节点，更新蛇头节点。
	**/
	temp = (point)malloc(sizeof(snake));
	temp->x= Head->x;temp->y = Head->y;         //更新蛇头节点
	temp->next = Head->next;temp->pre = Head;
	Head->next->pre = temp;Head->next = temp;
	temp = Tail;                                //去掉蛇尾节点
	Tail->pre->next = NULL;
	Tail = Tail->pre;
	free(temp);                                 //释放内存空间

	switch (snake_dir) {    //判断行进方向并打印蛇身
        case 'u': {
            Sleep(speedy);
            Head->y--;
            goto_xy(Head->x, Head->y + 1);
            printf("O");
        }
                  break;
        case 'd': {
            Sleep(speedy);
            Head->y++;
            goto_xy(Head->x, Head->y - 1);
            printf("O");
        }
                  break;
        case 'l': {
            Sleep(speedx);
            Head->x--;
            goto_xy(Head->x + 1, Head->y);
            printf("O");
        }
                  break;
        case 'r': {
            Sleep(speedx);
            Head->x++;
            goto_xy(Head->x - 1, Head->y);
            printf("O");
        }
                  break;
	}
	goto_xy(Head->x, Head->y);
	printf("@");                //打印蛇头
}

void change_dir_auto()  //自动改变方向
{
	former_dir = snake_dir;  //记录前一次移动的方向

    /**
    根据前一次移动的方向和蛇头的位置以及食物的位置
    在不违反规则的前提下根据判断确定一个可行的方向。
    **/

	if (Food[0] > Head->x)
	{
		if (snake_dir != 'l')
			snake_dir = 'r';
		else if (Head->y > 1)
			snake_dir = 'u';
		else
			snake_dir = 'd';
	}
	else if (Food[0] < Head->x)
	{
		if (snake_dir != 'r')
			snake_dir = 'l';
		else if (Head->y > 1)
			snake_dir = 'u';
		else
			snake_dir = 'd';
	}
	else if (Food[1] > Head->y)
	{
		if (snake_dir != 'u')
			snake_dir = 'd';
		else if (Head->x < 76)
			snake_dir = 'r';
		else
			snake_dir = 'l';

	}
	else if (Food[1] < Head->y)
	{
		if (snake_dir != 'd')
			snake_dir = 'u';
		else if (Head->x < 76)
			snake_dir = 'r';
		else
			snake_dir = 'l';
	}
}

void change_dir_mannul()
{
	short dir;
	dir = _getch();
	switch (dir)
	{
        case 72:
            if (snake_dir != 'd')
                snake_dir = 'u';
            break;
        case 80:
            if (snake_dir != 'u')
                snake_dir = 'd';
            break;
        case 75:
            if (snake_dir != 'r')
                snake_dir = 'l';
            break;
        case 77:
            if (snake_dir != 'l')
                snake_dir = 'r';
            break;
	}
}

void over_game()    //结束游戏
{
	//system("CLS");
	goto_xy(33, 12);
	system("color CF");     //设置背景颜色
	printf("Game Over!!!"); //打印结束信息
	write_history();        //记录历史
	getchar();
	goto_xy(22, 12);
	begin_game();           //重新初始化游戏
}

void win_game()     //游戏通关
{
	system("CLS");  //清屏
	goto_xy(22, 12);
	printf("You win the game.Congratulations!!!");  //打印祝贺信息
	getchar();
	begin_game();   //重新初始化游戏
}

void test()     //测试接口
{
	short t1, t2;
	system("color 1F");
	while (1)
	{
		while (_kbhit() != 1)
		{
			printf("HEHEHHE\n");
			Sleep(200);
		}
		t1 = _getch();
		t2 = _getch();
		printf("%d        %d \n", t1, t2);
	}
}

void write_history()        //记录历史
{
	time(&t);
	lt = localtime(&t);                         //获取当前系统时间
	HistoryFile = fopen("history.txt", "a");    //打开游戏记录文件
	if (a_or_m == 49)                           //写入游戏记录
	    fprintf(HistoryFile, "%scomputer    %5d\n", asctime(lt),history.current_score);
	else
		fprintf(HistoryFile, "%splayer      %5d\n", asctime(lt),history.current_score);
    fclose(HistoryFile);                        //关闭游戏记录文件

    for(int tmp = 0;tmp < 5;tmp ++)             //更新最高分
    {
       if (history.highest[tmp] < history.current_score)
        {
            history.highest[tmp] = history.current_score;
            RankFile = fopen("rank.txt", "w");   //更新高分记录文件
            for(int tmp1=0;tmp1<5;tmp1++)
                fprintf(RankFile,"%d\n",history.highest[tmp1]);
            fclose(RankFile);
            break;
        }
    }
};


void goto_xy(SHORT x, SHORT y)      //调用系统接口实现移动光标位置
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(hOut, pos);
}

