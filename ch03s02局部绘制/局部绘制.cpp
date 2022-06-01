#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

bool gameOver;
int fruitFlash;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail = 1;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int x, int y)//类似于清屏函数
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void Initial()
{
	//srand(time(NULL));
	gameOver = false;
	fruitFlash = 0;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

//光标位置
void setPos(int X, int Y)
{
	COORD pos;
	pos.X = X + 1;
	pos.Y = Y + 1;
	SetConsoleCursorPosition(h, pos);
}

void DrawMap()
{
	//上墙
	setPos(-1, -1);
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		cout << "#";

	//左右墙
	for (int i = 0; i < height; i++)
	{
		setPos(-1, i);
		for (int j = 0; j < width + 2; j++)
		{
			if (j == 0 || j == width + 1)
				cout << "#";
			else
				cout << " ";
		}
		cout << endl;
	}

	//下墙
	setPos(-1, height);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	cout << "游戏得分" << score << endl;
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
			//方向键
		case 224:
		{
			switch (_getch())
			{
			case 72:
			{
				if (dir != DOWN)
					dir = UP;
				break;
			}
			case 80:
			{
				if (dir != UP)
					dir = DOWN;
				break;
			}
			case 75:
			{
				if (dir != RIGHT)
					dir = LEFT;
				break;
			}
			case 77:
			{
				if (dir != LEFT)
					dir = RIGHT;
				break;
			}
			default:
				break;
			}
			break;
		}
		//WASD
		case 'a':
		{
			if (dir != RIGHT)
				dir = LEFT;
			break;
		}
		case 'd':
		{
			if (dir != LEFT)
				dir = RIGHT;
			break;
		}
		case 'w':
		{
			if (dir != DOWN)
				dir = UP;
			break;
		}
		case 's':
		{
			if (dir != UP)
				dir = DOWN;
			break;
		}
		case 'x':
		{
			gameOver = true;
			break;
		}
		default:
			break;
		}
	}
}

void eraseSnake()
{
	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		cout << " ";
	}
}

void Logic()
{
	if (x == fruitX && y == fruitY)
	{
		score++;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

	int prevX = tailX[0], prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir)
	{
	case LEFT:
	{
		x--;
		break;
	}
	case RIGHT:
	{
		x++;
		break;
	}
	case UP:
	{
		y--;
		break;
	}
	case DOWN:
	{
		y++;
		break;
	}
	}

	//if (x > width || x<0 || y>height || y < 0)
	//	gameOver = true;

	for (int i = 1; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	if (x >= width)
		x = 0;
	else if (x < 0)
		x = width - 1;
	if (y >= height)
		y = 0;
	else if (y < 0)
		y = height - 1;
}

void DrawLocally()
{
	fruitFlash++;
	if (fruitFlash)
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		cout << "F";

		if (fruitFlash >= 4)
			fruitFlash = -1;
	}
	else
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		cout << " ";
	}

	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		if (i == 0)
		{
			SetConsoleTextAttribute(h, 0x09);
			cout << "O";
		}
		else
		{
			SetConsoleTextAttribute(h, 0x0a);
			cout << "o";
		}
	}
	setPos(0, height + 1);
	SetConsoleTextAttribute(h, 0x06);
	cout << "游戏得分：" << score;
}

int main()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //隐藏光标

	Initial();
	DrawMap();
	while (!gameOver)
	{
		Input();
		eraseSnake();
		Logic();
		DrawLocally();
		Sleep(177);
	}

	//system("pause");
	Sleep(2000);
	return 0;
}