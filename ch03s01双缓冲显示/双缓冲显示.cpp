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

//��������������
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
		GENERIC_WRITE,//������̿�����������д����
		FILE_SHARE_WRITE,//���建�����ɹ���дȨ��
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
	//���������������Ĺ��
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

	//��ǽ
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//��ǽ
			SetConsoleTextAttribute(h, 0X06);
			if (j == 0)
				cout << "#";

			//��ͷ
			if (i == y && j == x)
			{
				SetConsoleTextAttribute(h, 0X0a);
				cout << "0";
			}
			//����
			else if (i == fruitY && j == fruitX)
			{
				SetConsoleTextAttribute(h, 0X04);
				cout << "F";
			}
			else
			{
				//����
				bool flagPrint = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						flagPrint = true;
					}
				}

				//����
				if (!flagPrint)
				{
					SetConsoleTextAttribute(h, 0X06);
					cout << " ";
				}
			}

			//��ǽ
			SetConsoleTextAttribute(h, 0X06);
			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}

	//��ǽ
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << endl;

	cout << "��Ϸ�÷�" << score << endl;
}

void Draw2()
{
	int currentLine = 0;

	//��ǽ
	for (int i = 0; i < width + 2; i++)
		ScreenData[currentLine][i] = '#';
	currentLine++;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			//��ǽ
			if (j == 0)
				ScreenData[currentLine + i][j] = '#';
			//��ͷ
			else if (i == y && j == x)
			{
				ScreenData[currentLine + i][j] = 'O';
			}
			//����
			else if (i == fruitY && j == fruitX)
			{
				ScreenData[currentLine + i][j] = 'F';
			}
			else
			{
				//����
				bool flagPrint = false;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						ScreenData[currentLine + i][j] = 'o';
						flagPrint = true;
					}
				}

				//����
				if (!flagPrint)
					ScreenData[currentLine + i][j] = ' ';
			}

			//��ǽ
			if (j == width - 1)
				ScreenData[currentLine + i][j] = '#';
		}
	}

	currentLine += height;
	//��ǽ
	for (int i = 0; i < width + 2; i++)
		ScreenData[currentLine][i] = '#';
	currentLine++;

	sprintf(ScreenData[currentLine], "��Ϸ�÷֣�%d", score);
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
	//��ScreenData��������ݽ�����ɫ���������Դ������������hBuf
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
		coord.X = 0;//��Ҫ�����µ�һ������ʱ�����coord.X��������
		//���洦����ǵ�i��ÿ���ַ�����ɫ���ԣ��·����봦����ǵ�i�������ַ�����
		WriteConsoleOutputCharacterA(hBuf, ScreenData[j], width, coord, &bytes);
	}
	//hBufָ�����ʾ����������д����ϡ������hBuf�����˫����Ļ��彻��
	SetConsoleActiveScreenBuffer(hBuf);
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
			//�����
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
	//�Թ���
	if (x == fruitX && y == fruitY)
	{
		score++;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

	//�����ƶ�
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

	//��ͷ�ƶ�
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

	//��ͷײ����
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	//��ǽ�ƶ�
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