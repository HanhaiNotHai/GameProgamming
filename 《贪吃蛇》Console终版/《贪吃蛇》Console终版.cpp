#include <iostream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include "unistd.h"
#include <sys/types.h>

using namespace std;

int FRAMES_PER_SECOND = 5;
double SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
DWORD next_Game_Tick = GetTickCount();
int sleep_Time = 0;

bool gameOver, isFullWidth = true, isFullChange, isPause, isAuto, isHide;
int fruitFlash;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100] = { 0 }, tailY[100] = { 0 };
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, next_dir;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void Initial()
{
	SetConsoleTitleA("��̰���ߡ�Console�հ�");
	COORD dsize = { 80,25 };
	SetConsoleScreenBufferSize(h, dsize);
	CONSOLE_CURSOR_INFO _cursor = { 1,false };
	SetConsoleCursorInfo(h, &_cursor);

	FRAMES_PER_SECOND = 5;
	SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	next_Game_Tick = GetTickCount();

	srand(time(NULL));
	//srand(getpid());
	gameOver = false;
	isFullChange = false;
	isPause = false;
	isHide = false;
	isAuto = false;
	fruitFlash = 0;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;

	nTail = 1;
}

//���λ��
void setPos(int X, int Y)
{
	COORD pos;
	if (isFullWidth)
		pos.X = 2 * X + 2;
	else
		pos.X = X + 2;
	pos.Y = Y + 2;
	SetConsoleCursorPosition(h, pos);
}

void DrawMap()
{
	system("cls");
	//��ǽ
	setPos(-1, -1);
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		if (isFullWidth)
			cout << "��";//���˫�ֽ��ַ�
		else
			cout << "#";

	//����ǽ
	for (int i = 0; i < height; i++)
	{
		setPos(-1, i);
		for (int j = 0; j < width + 2; j++)
		{
			if (j == 0 || j == width + 1)
				if (isFullWidth)
					cout << "��";//���˫�ֽ��ַ�
				else
					cout << "#";
			else
				if (isFullWidth)
					cout << "  ";//���˫�ֽ��ַ�
				else
					cout << " ";
		}
		cout << endl;
	}

	//��ǽ
	setPos(-1, height);
	for (int i = 0; i < width + 2; i++)
		if (isFullWidth)
			cout << "��";//���˫�ֽ��ַ�
		else
			cout << "#";
	cout << endl;
}

void Prompt_info(int _x, int _y)
{
	int initialX = 20, initialY = 0;

	SetConsoleTextAttribute(h, 0x0f);

	setPos(_x + initialX, _y + initialY);
	cout << "����Ϸ˵����";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	A.������ײ����Ϸ����";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	B.�߿ɴ�ǽ";
	initialY++;
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "������˵����";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	�������ƶ�����A";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	�������ƶ�����D";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	�������ƶ�����W";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	�������ƶ�����S";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	����ʼ��Ϸ�����ⷽ���";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	���˳���Ϸ��x";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	����ͣ���ո�";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	���������1";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	���Զ�Ѱ·��2";
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "	��ȫ����л���Enter";
	initialY++;
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "�����ߣ�������ý �����";
}

void Input()
{
	if (_kbhit())
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
					//dir = UP;
					next_dir = UP;
				break;
			}
			case 80:
			{
				if (dir != UP)
					//dir = DOWN;
					next_dir = DOWN;
				break;
			}
			case 75:
			{
				if (dir != RIGHT)
					//dir = LEFT;
					next_dir = LEFT;
				break;
			}
			case 77:
			{
				if (dir != LEFT)
					//dir = RIGHT;
					next_dir = RIGHT;
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
				//dir = LEFT;
				next_dir = LEFT;
			break;
		}
		case 'd':
		{
			if (dir != LEFT)
				//dir = RIGHT;
				next_dir = RIGHT;
			break;
		}
		case 'w':
		{
			if (dir != DOWN)
				//dir = UP;
				next_dir = UP;
			break;
		}
		case 's':
		{
			if (dir != UP)
				//dir = DOWN;
				next_dir = DOWN;
			break;
		}
		case 'x':
		{
			gameOver = true;
			break;
		}
		case 'f':
		{
			nTail++;
			score++;
			break;
		}
		case 'e':
		{
			FRAMES_PER_SECOND += 5;
			SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
			break;
		}
		case 'q':
		{
			FRAMES_PER_SECOND -= 5;
			if (FRAMES_PER_SECOND <= 0)
				FRAMES_PER_SECOND = 1;
			SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
			break;
		}
		case '1':
		{
			isHide = !isHide;
			break;
		}
		case '2':
		{
			isAuto = !isAuto;
			break;
		}
		case 0x0d:
		{
			isFullWidth = !isFullWidth;
			isFullChange = true;
			break;
		}
		case ' ':
		{
			isPause = true;
			break;
		}
		default:
			break;
		}
}

void pauseInput()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 0x0d:
		{
			isFullWidth = !isFullWidth;
			isFullChange = true;
			break;
		}
		case ' ':
		{
			isPause = false;
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
		if (isFullWidth)
			cout << "  ";//���ո�
		else
			cout << " ";
	}
}

//void show()
//{
//	char ch[height][width] = { ' ' };
//	ch[fruitX][fruitY] = 'F';
//	for (int i = 0; i < nTail; i++)
//	{
//		ch[tailY[i]][tailX[i]] = '0';
//	}
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			cout << ch[i][j];
//		}
//		cout << endl;
//	}
//}

//void halfAuto()
//{
//	if (y == 0)
//	{
//		if (dir == UP)
//			dir = LEFT;
//		else if (dir == LEFT)
//			dir = DOWN;
//	}
//	else if (y == height - 1)
//	{
//		if (dir == DOWN)
//			dir = LEFT;
//		else if (dir == LEFT)
//			dir = UP;
//	}
//}

bool zhuang(eDirection zdir)
{
	switch (zdir)
	{
	case LEFT:
		for (int i = 1; i < nTail; i++)
		{
			if (tailY[i] == y && (tailX[i] == x - 1 || x == 0 && tailX[i] == width - 1))
			{
				return true;
			}
		}
		return false;
		break;
	case RIGHT:
		for (int i = 1; i < nTail; i++)
		{
			if (tailY[i] == y && (tailX[i] == x + 1 || x == width - 1 && tailX[i] == 0))
			{
				return true;
			}
		}
		return false;
		break;
	case UP:
		for (int i = 1; i < nTail; i++)
		{
			if (tailX[i] == x && (tailY[i] == y - 1 || y == 0 && tailY[i] == height - 1))
			{
				return true;
			}
		}
		return false;
		break;
	case DOWN:
		for (int i = 1; i < nTail; i++)
		{
			if (tailX[i] == x && (tailY[i] == y + 1 || y == height - 1 && tailY[i] == 0))
			{
				return true;
			}
		}
		return false;
		break;
	default:
		return false;
		break;
	}
}

void allAuto()
{
	int deltaX, deltaY;
	if (abs(fruitX - x) <= width / 2)
		deltaX = abs(fruitX - x);
	else
		if (x < fruitX)
			deltaX = x + width - fruitX;
		else
			deltaX = fruitX + width - x;
	if (abs(fruitY - y) <= height / 2)
		deltaY = abs(fruitY - y) <= height / 2;
	else
		if (y < fruitY)
			deltaY = y + height - fruitY;
		else
			deltaY = fruitY + height - y;

	if (deltaX <= deltaY)
	{
		if (x != fruitX)
		{
			if (zhuang(LEFT))
			{
				if (dir != LEFT)
					next_dir = RIGHT;
			}
			else if (zhuang(RIGHT))
			{
				if (dir != RIGHT)
					next_dir = LEFT;
			}
			else
			{
				if (abs(fruitX - x) <= width / 2)
				{
					if (fruitX > x)
					{
						if (dir != LEFT)
							next_dir = RIGHT;
					}
					else
					{
						if (dir != RIGHT)
							next_dir = LEFT;
					}
				}
				else
				{
					if (fruitX > x)
					{
						if (dir != RIGHT)
							next_dir = LEFT;
					}
					else
					{
						if (dir != LEFT)
							next_dir = RIGHT;
					}
				}
			}
		}
		else if (y != fruitY)
		{
			if (zhuang(UP))
			{
				if (dir != UP)
					next_dir = DOWN;
			}
			else if (zhuang(DOWN))
			{
				if (dir != DOWN)
					next_dir = UP;
			}
			else
			{
				if (abs(fruitY - y) <= height / 2)
				{
					if (fruitY > y)
					{
						if (dir != UP)
							next_dir = DOWN;
					}
					else
					{
						if (dir != DOWN)
							next_dir = UP;
					}
				}
				else
				{
					if (fruitY > y)
					{
						if (dir != DOWN)
							next_dir = UP;
					}
					else
					{
						if (dir != UP)
							next_dir = DOWN;
					}
				}
			}
		}
	}
	else
	{
		if (y != fruitY)
		{
			if (zhuang(UP))
			{
				if (dir != UP)
					next_dir = DOWN;
			}
			else if (zhuang(DOWN))
			{
				if (dir != DOWN)
					next_dir = UP;
			}
			else
			{
				if (abs(fruitY - y) <= height / 2)
				{
					if (fruitY > y)
					{
						if (dir != UP)
							next_dir = DOWN;
					}
					else
					{
						if (dir != DOWN)
							next_dir = UP;
					}
				}
				else
				{
					if (fruitY > y)
					{
						if (dir != DOWN)
							next_dir = UP;
					}
					else
					{
						if (dir != UP)
							next_dir = DOWN;
					}
				}
			}
		}
		else if (x != fruitX)
		{
			if (zhuang(LEFT))
			{
				if (dir != LEFT)
					next_dir = RIGHT;
			}
			else if (zhuang(RIGHT))
			{
				if (dir != RIGHT)
					next_dir = LEFT;
			}
			else
			{
				if (abs(fruitX - x) <= width / 2)
				{
					if (fruitX > x)
					{
						if (dir != LEFT)
							next_dir = RIGHT;
					}
					else
					{
						if (dir != RIGHT)
							next_dir = LEFT;
					}
				}
				else
				{
					if (fruitX > x)
					{
						if (dir != RIGHT)
							next_dir = LEFT;
					}
					else
					{
						if (dir != LEFT)
							next_dir = RIGHT;
					}
				}
			}
		}
	}
}

void hide()
{
	if (zhuang(next_dir))
	{
		setPos(-1, 22);
		cout << "zhuang next_dir=" << next_dir << endl;
		if (next_dir == dir)
		{
			cout << "next_dir==dir" << endl;
			switch (dir)
			{
			case LEFT:
			case RIGHT:
				if (zhuang(UP))
					next_dir = DOWN;
				else if (zhuang(DOWN))
					next_dir = UP;
				else
				{
					for (int i = 1; i < nTail; i++)
						if (tailX[i] == x)
						{
							if (tailY[i] > y)
								next_dir = UP;
							else
								next_dir = DOWN;
							break;
						}
				}
				if (next_dir == dir)
					next_dir = UP;
				break;
			case UP:
			case DOWN:
				if (zhuang(LEFT))
					next_dir = RIGHT;
				else if (zhuang(RIGHT))
					next_dir = LEFT;
				else
				{
					for (int i = 1; i < nTail; i++)
						if (tailY[i] == y)
						{
							if (tailX[i] > x)
								next_dir = LEFT;
							else
								next_dir = RIGHT;
							break;
						}
				}
				if (next_dir == dir)
					next_dir = LEFT;
				break;
			default:
				break;
			}
			cout << "next_dir=" << next_dir;
		}
		else
		{
			if (zhuang(dir))
			{
				cout << "zhuang dir=" << dir << endl;
				switch (dir)
				{
				case LEFT:
				case RIGHT:
					if (next_dir == UP)
						next_dir = DOWN;
					else
						next_dir = UP;
					break;
				case UP:
				case DOWN:
					if (next_dir == LEFT)
						next_dir = RIGHT;
					else
						next_dir = LEFT;
					break;
				default:
					break;
				}
				cout << "next_dir=" << next_dir;
			}
			else
			{
				cout << "next_dir=dir";
				next_dir = dir;
			}
		}
	}
}

void Logic()
{
	if (x == fruitX && y == fruitY)
	{
		score++;
		nTail++;
		if (score % 5 == 0)
			FRAMES_PER_SECOND += 1;
		SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

	L1:
		fruitX = rand() % width;
		fruitY = rand() % height;
		for (int i = 0; i < nTail; i++)
			if (fruitX == tailX[i] && fruitY == tailY[i])
				goto L1;
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

	//halfAuto();

	if (isAuto)
		allAuto();
	if (isHide)
		hide();
	dir = next_dir;

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

	if (x > width - 1)
		x = 0;
	else if (x < 0)
		x = width - 1;
	if (y > height - 1)
		y = 0;
	else if (y < 0)
		y = height - 1;
}

void DrawLocally()
{
	if (isFullChange)
	{
		DrawMap();
		Prompt_info(3, 1);
		isFullChange = false;
	}

	fruitFlash++;
	if (fruitFlash)
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		if (isFullWidth)
			cout << "��";
		else
			cout << "F";

		if (fruitFlash >= 4)
			fruitFlash = -1;
	}
	else
	{
		setPos(fruitX, fruitY);
		SetConsoleTextAttribute(h, 0x04);
		if (isFullWidth)
			cout << "  ";
		else
			cout << " ";
	}

	for (int i = 0; i < nTail; i++)
	{
		setPos(tailX[i], tailY[i]);
		if (i == 0)
		{
			SetConsoleTextAttribute(h, 0x09);
			if (isFullWidth)
				cout << "��";
			else
				cout << "O";
		}
		else
		{
			SetConsoleTextAttribute(h, 0x09);
			if (isFullWidth)
				cout << "��";
			else
				cout << "o";
		}
	}

	/*setPos(0, height + 1);
	SetConsoleTextAttribute(h, 0x06);
	cout << "��Ϸ�÷֣�" << score;*/
}

void showScore(int _x, int _y)
{
	setPos(_x + 20, _y + 18);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "��ǰ���֣�";
	SetConsoleTextAttribute(h, 0x09);
	cout << score;

	setPos(_x + 20, _y + 19);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "��ǰ�Ѷȣ�";
	SetConsoleTextAttribute(h, 0x0c);
	cout << FRAMES_PER_SECOND << "  ";

	setPos(_x + 20, _y + 20);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "��ǰģʽ��";
	SetConsoleTextAttribute(h, 0x0c);
	if (!isHide)
		cout << "��";
	cout << "���";
	if (!isAuto)
		cout << "��";
	cout << "�Զ�    ";
}

void gameOver_info()
{
	if (isFullWidth)
		setPos(width / 2 - 3, height / 2 - 1);
	else
		setPos(width / 2 - 5, height / 2 - 1);
	SetConsoleTextAttribute(h, 0xec);
	cout << "��Ϸ������";

	if (isFullWidth)
		setPos(width / 2 - 4, height / 2);
	else
		setPos(width / 2 - 7, height / 2);
	cout << "Y���¿�ʼ/N�˳�";
}

int main()
{
	bool gameQuit = false;

	do
	{
		Initial();
		DrawMap();
		Prompt_info(3, 1);

		//��Ϸѭ��
		while (!gameOver)
		{
			if (!isPause)
				Input();
			else
				pauseInput();
			eraseSnake();
			if (!isPause)
				Logic();
			DrawLocally();
			showScore(3, 1);

			next_Game_Tick += SKIP_TICKS;
			sleep_Time = next_Game_Tick - GetTickCount();
			if (sleep_Time > 0)
				Sleep(sleep_Time);
		}

		gameOver_info();

		//�Ƿ��ؿ�
		while (gameOver)
		{
			if (_kbhit)
			{
				switch (_getch())
				{
				case'y':
				{
					gameOver = false;
					SetConsoleTextAttribute(h, 0X0F);
					system("cls");
					break;
				}
				case'n':
				{
					gameOver = false;
					gameQuit = true;
					break;
				}
				default:
					break;
				}
			}
		}
		Sleep(50);
	} while (!gameQuit);

	setPos(0, height + 3);
	//system("pause");
	return 0;
}