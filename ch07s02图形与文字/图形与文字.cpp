#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>

#define WINDOWS_WIDTH 80
#define WINDOWS_HEIGHT 25
#define STAGE_WIDTH 20
#define STAGE_HEIGHT 20
#define GRIDSIZE 25
#define SCALE 0.5
#define MAXLENGTH 400
#define INFO_WIDTH 400

using namespace sf;

bool gameOver;
int delay;
const int width = STAGE_HEIGHT;
const int height = STAGE_HEIGHT;
int x, y, fruitX, fruitY, score;
int tailX[MAXLENGTH] = { 0 }, tailY[MAXLENGTH] = { 0 };
int nTail, headRotation;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

RenderWindow window(VideoMode((width + 2)* GRIDSIZE + INFO_WIDTH, (height + 2)* GRIDSIZE), L"Snake by �����");
Texture tBackground, tSnakeHead, tSnakeBody, tFruit;
Sprite spBackground, spSnakeHead, spSnakeBody, spFruit;
Font font;
Text text;

void Initial()
{
	window.setFramerateLimit(60);

	if (!tBackground.loadFromFile("../̰�����ز�data/images/BK.png"))
		std::cout << "BK.pngû���ҵ�" << std::endl;
	if (!tSnakeHead.loadFromFile("../̰�����ز�data/images/sh01.png"))
		std::cout << "sh01.pngû���ҵ�" << std::endl;
	if (!tSnakeBody.loadFromFile("../̰�����ز�data/images/sb0102.png"))
		std::cout << "sb0102.pngû���ҵ�" << std::endl;
	if (!tFruit.loadFromFile("../̰�����ز�data/images/sb0202.png"))
		std::cout << "sb0202.pngû���ҵ�" << std::endl;

	spBackground.setTexture(tBackground);
	spSnakeHead.setTexture(tSnakeHead);
	spSnakeBody.setTexture(tSnakeBody);
	spFruit.setTexture(tFruit);

	spBackground.setScale(SCALE, SCALE);
	spSnakeHead.setScale(SCALE, SCALE);
	spSnakeBody.setScale(SCALE, SCALE);
	spFruit.setScale(SCALE, SCALE);

	spBackground.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spSnakeHead.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spSnakeBody.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);
	spFruit.setOrigin(GRIDSIZE / SCALE / 2, GRIDSIZE / SCALE / 2);

	if (!font.loadFromFile("C:/Windows/Fonts/simkai.ttf"))
		std::cout << "����û���ҵ�" << std::endl;

	text.setFont(font);

	gameOver = false;
	delay = 0;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;

	nTail = 1;
	headRotation = 0;
}

void Prompt_info(int _x, int _y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 24;
	text.setCharacterSize(24);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"����Ϸ˵����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	A.������ײ����Ϸ����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	B.�߿ɴ�ǽ");
	window.draw(text);
	initialY += CharacterSize * 2;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"������˵����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	�������ƶ�����A");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	�������ƶ�����D");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	�������ƶ�����W");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	�������ƶ�����S");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	����ʼ��Ϸ�����ⷽ���");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	���˳���Ϸ��x");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	����ͣ���ո�");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	��ȫ����л���Enter");
	window.draw(text);
	initialY += CharacterSize * 2;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"�����ߣ�������ý �����");
	window.draw(text);
}

void Input()
{
	Event event;
	while (window.pollEvent(event))
		if (event.type == Event::Closed)
			window.close();

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		if (dir != RIGHT)
			dir = LEFT;
		return;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		if (dir != LEFT)
			dir = RIGHT;
		return;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
	{
		if (dir != DOWN)
			dir = UP;
		return;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
	{
		if (dir != UP)
			dir = DOWN;
		return;
	}
	if (Keyboard::isKeyPressed(Keyboard::X))
	{
		gameOver = true;
		return;
	}
}

void Auto()
{
	if (y == 0)
	{
		if (dir == UP)
			dir = LEFT;
		else if (dir == LEFT)
			dir = DOWN;
	}
	else if (y == height - 1)
	{
		if (dir == DOWN)
			dir = LEFT;
		else if (dir == LEFT)
			dir = UP;
	}
}

void Logic()
{
	if (x == fruitX && y == fruitY)
	{
		score++;
		nTail++;

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

	//Auto();

	switch (dir)
	{
	case LEFT:
	{
		x--;
		headRotation = -90;
		break;
	}
	case RIGHT:
	{
		x++;
		headRotation = 90;
		break;
	}
	case UP:
	{
		y--;
		headRotation = 0;
		break;
	}
	case DOWN:
	{
		y++;
		headRotation = 180;
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

void Draw()
{
	window.clear(Color(255, 0, 255, 255));
	Prompt_info((width + 1) * GRIDSIZE, GRIDSIZE);

	int deltaX = GRIDSIZE / SCALE / 2;
	int deltaY = GRIDSIZE / SCALE / 2;

	//���Ʊ���
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			spBackground.setPosition(i * GRIDSIZE + deltaX, j * GRIDSIZE + deltaY);
			window.draw(spBackground);
		}
	}

	//������
	spSnakeHead.setPosition(tailX[0] * GRIDSIZE + deltaX, tailY[0] * GRIDSIZE + deltaY);
	spSnakeHead.setRotation(headRotation);
	window.draw(spSnakeHead);
	for (int i = 1; i < nTail; i++)
	{
		spSnakeBody.setPosition(tailX[i] * GRIDSIZE + deltaX, tailY[i] * GRIDSIZE + deltaY);
		window.draw(spSnakeBody);
	}

	//����ˮ��
	spFruit.setPosition(fruitX * GRIDSIZE + deltaX, fruitY * GRIDSIZE + deltaY);
	window.draw(spFruit);

	window.display();
}

int main()
{
	Initial();
	while (window.isOpen() && gameOver == false)
	{
		Input();
		delay++;
		if (delay % 10 == 0)
		{
			delay = 0;
			Logic();
		}
		Draw();
	}

	return 0;
}