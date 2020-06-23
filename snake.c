#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define keyW 119
#define keyA 97
#define keyS 115
#define keyD 100

int nScreenWidth = 40;
int nScreenHeight = 20;

struct Snake {
	int tail[800][2];
	int lenth;
	int napr;
};

struct Apple {
	int x;
	int y;
	int flag;
};

void snakeMove(struct Snake* snake, struct Apple* apple)
{
	int key;
	if(kbhit())
		key = getch();
		
	if(key == keyW && snake->napr != 2)
		snake->napr = 0;
	if(key == keyA && snake->napr != 3)
		snake->napr = 1;
	if(key == keyS && snake->napr != 0)
		snake->napr = 2;
	if(key == keyD && snake->napr != 1)
		snake->napr = 3;

	for(int i = snake->lenth; i > 0; i--)
	{
		snake->tail[i][0] = snake->tail[i - 1][0];
		snake->tail[i][1] = snake->tail[i - 1][1];
	}
		
	if(snake->napr == 0)
		(snake->tail[0][1])--;
	if(snake->napr == 1)
		(snake->tail[0][0])--;
	if(snake->napr == 2)
		(snake->tail[0][1])++;
	if(snake->napr == 3)
		(snake->tail[0][0])++;
}

void addSnake(struct Snake *snake, char *screen, WORD *attribute)
{
	screen[snake->tail[0][1]*nScreenWidth + snake->tail[0][0]] = 'O';
	attribute[snake->tail[0][1]*nScreenWidth + snake->tail[0][0]] = FOREGROUND_GREEN;
	for(int i = 1; i <= snake->lenth; i++)
	{
		if(snake->tail[i][0] != -1)
		{
			screen[snake->tail[i][1]*nScreenWidth + snake->tail[i][0]] = 'o';
			attribute[snake->tail[i][1]*nScreenWidth+snake->tail[i][0]] = FOREGROUND_GREEN;
		}
	}
}

void removeSnake(struct Snake *snake, char *screen, WORD *attribute)
{
	for(int i = 0; i <= snake->lenth; i++)
	{
		if(snake->tail[i][0] != -1)
		{	
			screen[snake->tail[i][1]*nScreenWidth + snake->tail[i][0]] = '\0';
			attribute[snake->tail[i][1]*nScreenWidth+snake->tail[i][0]] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}
}

void createApple(struct Apple* apple, char* screen, WORD *attribute)
{
	while(apple->flag == 0)
	{
		apple->x = rand()%(nScreenWidth-2)+1;
		apple->y = rand()%(nScreenHeight-2)+1;
		if(screen[apple->y*nScreenWidth + apple->x] != 'o')
			apple->flag = 1;
	}
	screen[apple->y*nScreenWidth + apple->x] = '@';
	attribute[apple->y*nScreenWidth + apple->x] = FOREGROUND_RED | FOREGROUND_INTENSITY;
}
void eatApple(struct Snake* snake, struct Apple* apple, WORD *attribute)
{
	if(snake->tail[0][0] == apple->x && snake->tail[0][1] == apple->y)
	{
		apple->flag = 0;
		attribute[apple->y*nScreenWidth + apple->x] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		(snake->lenth)++;
	}
	
}
void gameOver(struct Snake* snake)
{
	if(snake->tail[0][0] == 0 || snake->tail[0][1] == 0 || snake->tail[0][0] == 39 || snake->tail[0][1] == 19)
	{
		getch();
		exit(0);
	}
	for(int i = 1; i <= snake->lenth; i++)
	{
		if(snake->tail[0][0] == snake->tail[i][0] && snake->tail[0][1] == snake->tail[i][1])
		{
			getch();
			exit(0);
		}
	}
}

int main()
{
	system("mode 40, 20");
	srand(time(0));
	char screen[nScreenWidth*nScreenHeight + 1];	//строка символов
	WORD attribute[nScreenWidth*nScreenHeight + 1];	//строка атрибутов
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	COORD bufferSize = {40, 20};
	SetConsoleScreenBufferSize(hConsole, bufferSize);	//установить размер буфера экрана 40 на 20
	SetConsoleActiveScreenBuffer(hConsole);	//сделать буфер экрана hConsole активыным
	DWORD dwBytesWritten = 0;
	screen[nScreenWidth*nScreenHeight] = '\0';
	attribute[nScreenWidth*nScreenHeight] = '\0';
	COORD coord = {0, 0};
	
	CONSOLE_CURSOR_INFO structCursorInfo;	//сделать
    GetConsoleCursorInfo(hConsole, &structCursorInfo);	//курсор
    structCursorInfo.bVisible = FALSE;	//в консоли
	SetConsoleCursorInfo(hConsole, &structCursorInfo );	//невидимым
	
	//инициализация строки символов
	for(int i = 0; i < nScreenHeight; i++)
		for(int j = 0; j < nScreenWidth; j++)
			screen[i*nScreenWidth+j] = '\0';
	//инициализация строки атрибутов
	for(int i = 0; i < nScreenHeight; i++)
		for(int j = 0; j < nScreenWidth; j++)
			attribute[i*nScreenWidth+j] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	//рисование рамки
	for(int i = 0; i < nScreenWidth; i++)	
	{
		screen[0*nScreenWidth + i] = '#';
		screen[19*nScreenWidth + i] = '#';
	}
	for(int i = 0; i < nScreenWidth; i++)
	{
		screen[i*nScreenWidth + 0] = '#';
		screen[i*nScreenWidth + 39] = '#';
	}
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, coord, &dwBytesWritten);
	WriteConsoleOutputAttribute(hConsole, attribute, nScreenWidth*nScreenHeight, coord, &dwBytesWritten);
	//
	struct Snake snake;
	snake.tail[0][0] = 1;
	snake.tail[0][1] = 1;
	snake.lenth = 0;
	snake.napr = 3;
	for(int i = 1; i < 800; i++)
	{
		snake.tail[i][0] = -1;
		snake.tail[i][1] = -1;
	}
	struct Apple apple;
	apple.flag = 0;
	//игровой цикл
	while(1)
	{
		createApple(&apple, screen, attribute);
		
		removeSnake(&snake, screen, attribute);
		
		eatApple(&snake, &apple, attribute);
		snakeMove(&snake, &apple);
		
		gameOver(&snake);
		
		addSnake(&snake, screen, attribute);
		
		if(snake.napr == 0 || snake.napr == 2)
			Sleep(200);
		else
			Sleep(100);
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, coord, &dwBytesWritten);
		WriteConsoleOutputAttribute(hConsole, attribute, nScreenWidth*nScreenHeight, coord, &dwBytesWritten);
	}
}
