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
point Head, Tail, temp,te;
FILE * RankFile,*HistoryFile;

	short snake_lenth, Food[2];
	char  snake_dir = 'd', former_dir;
	void goto_xy(SHORT, SHORT);
	void over_game(void);
	void begin_game(void);
	void win_game(void);
	void draw_map(void);
	void draw_snake(void);
	void draw_menu(void);
	void draw_food(void);
	void write_history();
	void change_dir_auto(void);
	void change_dir_mannul(void);
	void move_snake();
	short check_game();
	void test(void);
	void begin_game(void);
	short game, a_or_m, speedx, speedy;


int main(int argc, char *argv[])
{
//	test();
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cinfo;
	cinfo.bVisible = 0;
	cinfo.dwSize = 1;
	SetConsoleCursorInfo(fd, &cinfo);
	begin_game();

	while (1) {
		game = check_game();
		if (game == 1) {
			temp = (point)malloc(sizeof(snake));
			temp->x = Tail->x;temp->y = Tail->y;
			Tail->next = temp;
			temp->pre = Tail;
			Tail = temp;
			goto_xy(84, 8);
			history.current_score += 10;
			printf("%5d",history.current_score);
			draw_food();
			move_snake();
			snake_lenth++;
		}
		else if (game == 2)
			move_snake();
		else if (game == 0)
			over_game();
		else if (game == 3)
			win_game();
		else if (game == 4)
		{
			system("CLS");
			goto_xy(33, 12);
			printf("See U next time!!!");
			break;
		}
	}
	getchar();
}

void draw_map()
{
	goto_xy(0, 0);
	short tmp;
	for (tmp = 1; tmp <= 78; tmp++)
		printf("_");
	printf("\n");
	for (tmp = 1; tmp <= 22; tmp++)
		printf("|                                                                            |\n");
	for (tmp = 1; tmp <= 78; tmp++)
		printf("-");
}

void draw_snake()
{
	goto_xy(1, 1);
	system("color 2E");
	printf("@@@*");
	temp = (point)malloc(sizeof(snake));
	temp->x = 4;temp->y = 1;
	Head = Tail = temp;
	temp = (point)malloc(sizeof(snake));
	temp->x = 3;temp->y = 1;
	Tail->next = temp;
	Tail = temp;
	Tail->pre = Head;
	temp = (point)malloc(sizeof(snake));
	temp->x = 2;temp->y = 1;

	Tail->next = temp;
	temp->pre = Tail;
	Tail = temp;
	temp = (point)malloc(sizeof(snake));
	temp->x = 1;temp->y = 1;

	Tail->next = temp;
	temp->pre = Tail;
	Tail = temp;

	Tail->next = NULL;
	Head->pre = NULL;
	snake_lenth = 4;
	snake_dir = 'r';
}

void begin_game()
{
	srand((unsigned)time(NULL));
	draw_menu();
	RankFile=fopen("rank.txt","r");
	for(int tmp=0;tmp<5;tmp++)
	fscanf(RankFile,"%d",&(history.highest[tmp]));

	fclose(RankFile);
	history.current_score = 0;
	if (game != 4)
	{
		system("color 1E");
		system("CLS");
		system("color 2E");
		draw_map();
		draw_snake();
		draw_food();

	}
	goto_xy(80,3);
	printf("highest score: ");
	goto_xy(84,4);
	printf("%5d",history.highest[0]);
	goto_xy(80, 7);
	printf("current score: ");
	goto_xy(84, 8);
	printf("%5d", history.current_score);
}

void draw_food()
{

	short tmp1, tmp2;
	tmp1 = rand() % 75 + 1;
	tmp2 = rand() % 22 + 1;
	te = Tail;
	while (te->pre != NULL)
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
	printf("$");
}

void draw_menu()
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
	a_or_m = _getch();
	_getch();
	if (a_or_m == 51)
		game = 4;
}

short check_game()
{
	if (game == 4)
		return 4;
	else if (snake_lenth == MAXLENTH)
		return 3;
	else if (Head->x == 77 || Head->y == 23 || Head->x == 0 || Head->y == 0)
		return 0;
	else if (Head->x == Food[0] && Head->y == Food[1])
		return 1;
	else
		return 2;
}

void move_snake() {
	short tmp;
	if (a_or_m == 49) {
		if (_kbhit() == 1)
		over_game();

		speedx = 1;
		speedy = 1;
		change_dir_auto();
	}
	else if (_kbhit() == 1) {
		speedx = 10;
		speedy = 10;
		change_dir_mannul();
	}
	else {
		speedx = 100;
		speedy = 150;
	}
	te = Tail;
	tmp = 1;
	while (te->pre != NULL)
	{
		if (Tail->x == te->pre->x && Tail->y == te->pre->y)
		{
			tmp = 0;
			break;
		}
		te = te->pre;
	}

	if (tmp != 0)
	{
		goto_xy(Tail->x, Tail->y);
		printf(" ");
	}

	te=Tail;

	temp = (point)malloc(sizeof(snake));
	temp->x= Head->x;temp->y = Head->y;
	temp->next = Head->next;temp->pre = Head;
	Head->next->pre = temp;Head->next = temp;
	temp = Tail;
	Tail->pre->next = NULL;
	Tail = Tail->pre;
	free(temp);


	switch (snake_dir) {
	case 'u': {
		Sleep(speedy);
		Head->y--;
		goto_xy(Head->x, Head->y + 1);
		printf("@");
	}
			  break;
	case 'd': {
		Sleep(speedy);
		Head->y++;
		goto_xy(Head->x, Head->y - 1);
		printf("@");
	}
			  break;
	case 'l': {
		Sleep(speedx);
		Head->x--;
		goto_xy(Head->x + 1, Head->y);
		printf("@");
	}
			  break;
	case 'r': {
		Sleep(speedx);
		Head->x++;
		goto_xy(Head->x - 1, Head->y);
		printf("@");
	}
			  break;
	}
	goto_xy(Head->x, Head->y);
	printf("*");

}

void change_dir_auto()
{
	former_dir = snake_dir;

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

void over_game() {
	//system("CLS");
	goto_xy(33, 12);
	system("color CF");
	printf("Game Over!!!");
	write_history();
	getchar();
	goto_xy(22, 12);
	begin_game();
}

void win_game()
{
	system("CLS");
	goto_xy(22, 12);
	printf("You win the game.Congratulations!!!");
	getchar();
	begin_game();
}

void test()
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

void write_history()
{
	time(&t);
	lt = localtime(&t);
	HistoryFile = fopen("history.txt", "a");
	if (a_or_m == 49)
	    fprintf(HistoryFile, "%scomputer    %5d\n", asctime(lt),history.current_score);
	else
		fprintf(HistoryFile, "%splayer      %5d\n", asctime(lt),history.current_score);
		fclose(HistoryFile);
		for(int tmp=0;tmp<5;tmp++)

    for(int tmp=0;tmp<5;tmp++)
   {
   if (history.highest[tmp]<history.current_score)
    {
        history.highest[tmp]=history.current_score;
        RankFile = fopen("rank.txt", "w");
        for(int tmp1=0;tmp1<5;tmp1++)
	    fprintf(RankFile,"%d\n",history.highest[tmp1]);
	    fclose(RankFile);
	    break;
    }
   }

};


void goto_xy(SHORT x, SHORT y)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(hOut, pos);
}

