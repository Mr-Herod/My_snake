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

	short snake_lenth, Food[2];         //��¼�����ȣ�ʳ������
	short game, a_or_m, speedx, speedy; //��¼��Ϸ״̬����Ϸģʽ���ƶ��ٶ�
	char  snake_dir = 'd', former_dir;  //��¼��ͷ�����ǰһ���ƶ��ķ���
	void goto_xy(SHORT, SHORT);         //�ƶ���굽x,yָʾ��������
	void over_game(void);               //������Ϸ
	void begin_game(void);              //��ʼ��Ϸ
	void win_game(void);                //Ӯ����Ϸ
	void draw_map(void);                //��ӡ��ͼ
	void draw_snake(void);              //��ӡ����
	void draw_menu(void);               //��ӡ�˵�
	void draw_food(void);               //��ӡʳ��
	void write_history();               //��¼��Ϸ��ʷ
	void change_dir_auto(void);         //�Զ��ı䷽��
	void change_dir_mannul(void);       //�ֶ��ı䷽��
	void move_snake();                  //�ƶ�����
	short check_game();                 //�����Ϸ״̬
	void test(void);                    //���Խӿ�


int main(int argc, char *argv[])        //��������ʼ
{
//	test();
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);//���ع��
	CONSOLE_CURSOR_INFO cinfo;                  //���ع��
	cinfo.bVisible = 0;                         //���ع��
	cinfo.dwSize = 1;                           //���ع��
	SetConsoleCursorInfo(fd, &cinfo);           //���ع��
	begin_game();                               //��ʼ��Ϸ

	while (1) {
		game = check_game();                    //�ж���Ϸ״̬
		if (game == 1) {                        //�Ե�ʳ���������
			temp = (point)malloc(sizeof(snake));//β�巨��������ڵ�
			temp->x = Tail->x;temp->y = Tail->y;
			Tail->next = temp;
			temp->pre = Tail;
			Tail = temp;
			goto_xy(84, 8);
			history.current_score += 10;        //�÷ּ�+10
			printf("%5d",history.current_score);//���µ÷�
			draw_food();                        //��������ʳ��
			move_snake();                       //�����ƶ�����
			snake_lenth++;                      //������+1
		}
		else if (game == 2)
			move_snake();                       //�ƶ�����
		else if (game == 0)
			over_game();                        //������Ϸ
		else if (game == 3)
			win_game();                         //Ӯ����Ϸ
		else if (game == 4)
		{
			system("CLS");                      //����
			goto_xy(33, 12);                    //�ƶ����
			printf("See U next time!!!");       //��ӡ������Ϣ
			break;                              //�˳���ѭ������������
		}
	}
	getchar();
}

void draw_map()    //��ӡ��ͼ
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

void draw_snake()    //��ӡ��ʼ����
{
	goto_xy(1, 1);
	system("color 2E");
	printf("OOO@");

	temp = (point)malloc(sizeof(snake));    //����һ����ͷ�ڵ�
	temp->x = 4;temp->y = 1;
	Head = Tail = temp;
    for(int i = 3 ; i > 0 ; i --)           //������������ڵ�
	{
	    temp = (point)malloc(sizeof(snake));
        temp->x = i;temp->y = 1;            //��¼��������
        temp->pre = Tail;
        Tail->next = temp;
        Tail = temp;
	}
	Tail->next = NULL;
	Head->pre = NULL;
	snake_lenth = 4;                        //�����ȳ�ʼ��Ϊ4
	snake_dir = 'r';                        //�����ߵĳ�ʼ����Ϊ��
}

void begin_game()   //��ʼ����Ϸ
{
	srand((unsigned)time(NULL));                    //�����������
	draw_menu();                                    //��ӡ��ͼ

	RankFile=fopen("rank.txt","r");                 //����ʷ��¼�ļ�
	for(int tmp=0;tmp<5;tmp++)                      //��ȡ��ʷ��¼
        fscanf(RankFile,"%d",&(history.highest[tmp]));
	fclose(RankFile);                               //�ر���ʷ��¼�ļ�

	history.current_score = 0;                      //��ʼ������
	if (game != 4)                                  //��ʼ����ͼ������ʳ��
	{
		system("color 1E");
		system("CLS");
		system("color 2E");
		draw_map();
		draw_snake();
		draw_food();
	}
	goto_xy(80,3);
	printf("highest score: ");                      //��ʾ��ʷ��߷�
	goto_xy(84,4);
	printf("%5d",history.highest[0]);               //��ʾ��ʷ��߷�
	goto_xy(80, 7);
	printf("current score: ");
	goto_xy(84, 8);
	printf("%5d", history.current_score);           //��ʾ��ǰ�÷�
}

void draw_food()    //��ӡʳ��
{
    /**
    �������ʳ�����꣬�ж��Ƿ���������
    ����ǣ�����������һ������
    ���򣬾ʹ�ӡʳ��
    **/
	short tmp1, tmp2;
	tmp1 = rand() % 75 + 1;
	tmp2 = rand() % 22 + 1;
	te = Tail;
	while (te->pre != NULL)     //�ж������ɵ������Ƿ��������ص�
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
	printf("$");                //��ӡʳ��
}

void draw_menu()                //��ӡ�˵�
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
	a_or_m = _getch();          //��ȡ����
	_getch();                   //��ȡѡ��
	if (a_or_m == 51)
		game = 4;
}

short check_game()              //�ж���Ϸ״̬
{
	if (game == 4)              //������Ϸ
		return 4;
	else if (snake_lenth == MAXLENTH)                   //����ͨ�أ�Ӯ����Ϸ
		return 3;
	else if (Head->x == 77 || Head->y == 23 || Head->x == 0 || Head->y == 0)    //ײǽ����
		return 0;
	else if (Head->x == Food[0] && Head->y == Food[1])  //�Ե�ʳ��
		return 1;
	else                                                //�����ƶ�
		return 2;
}

void move_snake() {         //�ƶ�����
	short tmp;
	if (a_or_m == 49) {     //�ж����ֶ�ģʽ�����Զ�ģʽ
		if (_kbhit() == 1)  //�Զ�ģʽ���м�������ͽ�����Ϸ
            over_game();    //������Ϸ
		speedx = 1;         //�����ٶ�
		speedy = 1;         //�����ٶ�
		change_dir_auto();  //�Զ��ı䷽��
	}
	else if (_kbhit() == 1) { //�ֶ�ģʽ�м����������ȡ���룬�ı䷽��
		//speedx = 10;
		//speedy = 10;
		change_dir_mannul();  //�ֶ��ı䷽��
		step = 0;
	}
	else {
		speedx = 100;         //�����ƶ��ٶ�
		speedy = 150;         //�����ƶ��ٶ�
	}

	if(pre_step != 0 && step ++ == 0)
        {pre_step = 0;return;}
    pre_step = step - 1;
	te = Tail;
	tmp = 1;
	while (te->pre != NULL)   //�ж���β�Ƿ��������ص�
	{
		if (Tail->x == te->pre->x && Tail->y == te->pre->y)
		{
			tmp = 0;
			break;
		}
		te = te->pre;
	}
	if (tmp != 0)              //û���ص�����β��Ϊ�հ�
	{
		goto_xy(Tail->x, Tail->y);
		printf(" ");
	}

	te=Tail;
	/**
	������������ȥ����β�ڵ㣬������ͷ�ڵ㡣
	**/
	temp = (point)malloc(sizeof(snake));
	temp->x= Head->x;temp->y = Head->y;         //������ͷ�ڵ�
	temp->next = Head->next;temp->pre = Head;
	Head->next->pre = temp;Head->next = temp;
	temp = Tail;                                //ȥ����β�ڵ�
	Tail->pre->next = NULL;
	Tail = Tail->pre;
	free(temp);                                 //�ͷ��ڴ�ռ�

	switch (snake_dir) {    //�ж��н����򲢴�ӡ����
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
	printf("@");                //��ӡ��ͷ
}

void change_dir_auto()  //�Զ��ı䷽��
{
	former_dir = snake_dir;  //��¼ǰһ���ƶ��ķ���

    /**
    ����ǰһ���ƶ��ķ������ͷ��λ���Լ�ʳ���λ��
    �ڲ�Υ�������ǰ���¸����ж�ȷ��һ�����еķ���
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

void over_game()    //������Ϸ
{
	//system("CLS");
	goto_xy(33, 12);
	system("color CF");     //���ñ�����ɫ
	printf("Game Over!!!"); //��ӡ������Ϣ
	write_history();        //��¼��ʷ
	getchar();
	goto_xy(22, 12);
	begin_game();           //���³�ʼ����Ϸ
}

void win_game()     //��Ϸͨ��
{
	system("CLS");  //����
	goto_xy(22, 12);
	printf("You win the game.Congratulations!!!");  //��ӡף����Ϣ
	getchar();
	begin_game();   //���³�ʼ����Ϸ
}

void test()     //���Խӿ�
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

void write_history()        //��¼��ʷ
{
	time(&t);
	lt = localtime(&t);                         //��ȡ��ǰϵͳʱ��
	HistoryFile = fopen("history.txt", "a");    //����Ϸ��¼�ļ�
	if (a_or_m == 49)                           //д����Ϸ��¼
	    fprintf(HistoryFile, "%scomputer    %5d\n", asctime(lt),history.current_score);
	else
		fprintf(HistoryFile, "%splayer      %5d\n", asctime(lt),history.current_score);
    fclose(HistoryFile);                        //�ر���Ϸ��¼�ļ�

    for(int tmp = 0;tmp < 5;tmp ++)             //������߷�
    {
       if (history.highest[tmp] < history.current_score)
        {
            history.highest[tmp] = history.current_score;
            RankFile = fopen("rank.txt", "w");   //���¸߷ּ�¼�ļ�
            for(int tmp1=0;tmp1<5;tmp1++)
                fprintf(RankFile,"%d\n",history.highest[tmp1]);
            fclose(RankFile);
            break;
        }
    }
};


void goto_xy(SHORT x, SHORT y)      //����ϵͳ�ӿ�ʵ���ƶ����λ��
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(hOut, pos);
}

