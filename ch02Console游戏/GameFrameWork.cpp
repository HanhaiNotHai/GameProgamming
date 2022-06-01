#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

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
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw()
{
	//system("cls");
	gotoxy(0, 0);
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int textColor;
	
	//上墙
	textColor = 0X06;
	SetConsoleTextAttribute(h, textColor);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//左墙
			textColor = 0X06;
			SetConsoleTextAttribute(h, textColor);
			if (j == 0)
				cout << "#";

			//蛇头
			if (i == y && j == x)
			{
				textColor = 0X0a;
				SetConsoleTextAttribute(h, textColor);
				cout << "0";
			}
			//果子
			else if (i == fruitY && j == fruitX)
			{
				textColor = 0X04;
				SetConsoleTextAttribute(h, textColor);
				cout << "F";
			}
			else
			{
				//蛇身
				bool flagPrint = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						flagPrint = true;
					}
				}

				//背景
				if (!flagPrint)
				{
					textColor = 0X06;
					SetConsoleTextAttribute(h, textColor);
					cout << " ";
				}
			}

			//右墙
			textColor = 0X06;
			SetConsoleTextAttribute(h, textColor);
			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	//下墙
	textColor = 0X06;
	SetConsoleTextAttribute(h, textColor);
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

	for (int i = 0; i < nTail; i++)
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

int main()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //隐藏光标

	Initial();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		Sleep(77);
	}

	//system("pause");
	Sleep(2000);
	return 0;
}