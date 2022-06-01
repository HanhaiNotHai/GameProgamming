#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

HANDLE hOutput, hOutBuf;
COORD coord = { 0,0 };
DWORD bytes = 0;
bool bufferSwapFlag = false;

bool gameOver;
const int width = 20;
const int height = 20;
char ScreenData[width + 5][height + 5];

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

//类似于清屏函数
void gotoxy(int x, int y)
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

	hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_WRITE,//定义进程可以往缓冲区写数据
		FILE_SHARE_WRITE,//定义缓冲区可共享写权限
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	hOutput = CreateConsoleScreenBuffer(
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	//隐藏两个缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	//CONSOLE_CURSOR_INFO cci = { 1,0 };
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hOutput, &cci);
	SetConsoleCursorInfo(hOutBuf, &cci);

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
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	//system("cls");
	gotoxy(0, 0);

	//上墙
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//左墙
			SetConsoleTextAttribute(h, 0X06);
			if (j == 0)
				cout << "#";

			//蛇头
			if (i == y && j == x)
			{
				SetConsoleTextAttribute(h, 0X0a);
				cout << "0";
			}
			//果子
			else if (i == fruitY && j == fruitX)
			{
				SetConsoleTextAttribute(h, 0X04);
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
					SetConsoleTextAttribute(h, 0X06);
					cout << " ";
				}
			}

			//右墙
			SetConsoleTextAttribute(h, 0X06);
			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	//下墙
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	cout << "游戏得分" << score << endl;
}

void Draw2()
{
	int currentLine = 0;

	//上墙
	for (int i = 0; i < width + 2; i++)
		ScreenData[currentLine][i] = '#';
	currentLine++;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			//左墙
			if (j == 0)
				ScreenData[currentLine + i][j] = '#';
			//蛇头
			else if (i == y && j == x)
			{
				ScreenData[currentLine + i][j] = 'O';
			}
			//果子
			else if (i == fruitY && j == fruitX)
			{
				ScreenData[currentLine + i][j] = 'F';
			}
			else
			{
				//蛇身
				bool flagPrint = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						ScreenData[currentLine + i][j] = 'o';
						flagPrint = true;
					}
				}

				//背景
				if (!flagPrint)
					ScreenData[currentLine + i][j] = ' ';
			}

			//右墙
			if (j == width - 1)
				ScreenData[currentLine + i][j] = '#';
		}
	}

	currentLine += height;
	//下墙
	for (int i = 0; i < width + 2; i++)
		ScreenData[currentLine][i] = '#';
	currentLine++;

	sprintf(ScreenData[currentLine], "游戏得分：%d", score);
}

void Show_doubleBuffer()
{
	HANDLE hBuf;
	WORD textColor;
	int i, j;

	Draw2();

	if (bufferSwapFlag == false)
	{
		bufferSwapFlag = true;
		hBuf = hOutBuf;
	}
	else
	{
		bufferSwapFlag = false;
		hBuf = hOutput;
	}
	//对ScreenData数组的内容进行上色，并将属性传到输出缓冲区hBuf
	for (j = 0; j < height + 5; j++)
	{
		coord.Y = j;
		for (i = 0; i < width + 5; i++)
		{
			coord.X = i;
			if (ScreenData[j][i] == 'O')
				textColor = 0x03;
			else if (ScreenData[j][i] == 'F')
				textColor = 0x04;
			else if (ScreenData[j][i] == 'o')
				textColor = 0x0a;
			else
				textColor = 0x06;
			WriteConsoleOutputAttribute(hBuf, &textColor, 1, coord, &bytes);
		}
		coord.X = 0;//若要处理新的一行内容时，需对coord.X重新置零
		//上面处理的是第i行每个字符的颜色属性，下方代码处理的是第i行整行字符内容
		WriteConsoleOutputCharacterA(hBuf, ScreenData[j], width, coord, &bytes);
	}
	//hBuf指向的显示缓冲区内容写入完毕。激活该hBuf，完成双缓冲的缓冲交换
	SetConsoleActiveScreenBuffer(hBuf);
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
	//吃果子
	if (x == fruitX && y == fruitY)
	{
		score++;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

	//蛇身移动
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

	//蛇头移动
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

	//蛇头撞蛇身
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	//穿墙移动
	if (x >= width - 1)
		x = 1;
	else if (x <= 0)
		x = width - 2;
	if (y >= height)
		y = 0;
	else if (y < 0)
		y = height - 1;
}

int main()
{
	Initial();
	while (!gameOver)
	{
		//Draw();
		Show_doubleBuffer();
		Input();
		Logic();
		Sleep(77);
	}

	/*HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(h);
	system("pause");*/
	Sleep(2000);
	return 0;
}