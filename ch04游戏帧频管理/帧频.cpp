#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

double FRAMES_PER_SECOND = 5;
int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
DWORD next_Game_Tick = GetTickCount();
int sleep_Time = 0;

bool gameOver, isPause;
int fruitFlash;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void Initial()
{
	SetConsoleTitleA("Console_̰����");
	COORD dsize = { 80,25 };
	SetConsoleScreenBufferSize(h, dsize);
	CONSOLE_CURSOR_INFO _cursor = { 1,false };
	SetConsoleCursorInfo(h, &_cursor);

	//srand(time(NULL));
	FRAMES_PER_SECOND = 5;
	SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	gameOver = false;
	isPause = false;
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
	pos.X = X + 1;
	pos.Y = Y + 1;
	SetConsoleCursorPosition(h, pos);
}

void DrawMap()
{
	//��ǽ
	setPos(-1, -1);
	SetConsoleTextAttribute(h, 0X06);
	for (int i = 0; i < width + 2; i++)
		cout << "#";

	//����ǽ
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

	//��ǽ
	setPos(-1, height);
	for (int i = 0; i < width + 2; i++)
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
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "����Ϸ�Ѷȣ�";
	initialY++;
	initialY++;

	setPos(_x + initialX, _y + initialY);
	cout << "�����ߣ�������ý �����";
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
		case ' ':
		{
			isPause = true;
			break;
		}
		default:
			break;
		}
	}
}

void pauseInput()
{
	if (_kbhit)
		switch (_getch())
		{
		case ' ':
			isPause = false;
			break;
		default:
			break;
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
		FRAMES_PER_SECOND += 0.5;
		SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
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

	setPos(35, 13);
	cout << FRAMES_PER_SECOND << "  ";

	/*setPos(0, height + 1);
	SetConsoleTextAttribute(h, 0x06);
	cout << "��Ϸ�÷֣�" << score;*/
}

void showScore(int _x, int _y)
{
	setPos(_x + 20, _y + 17);
	SetConsoleTextAttribute(h, 0x0f);
	cout << "��ǰ���֣�";
	SetConsoleTextAttribute(h, 0x0c);
	cout << score;
}

void gameOver_info()
{
	setPos(5, 8);
	SetConsoleTextAttribute(h, 0xec);
	cout << "��Ϸ������";
	setPos(3, 9);
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