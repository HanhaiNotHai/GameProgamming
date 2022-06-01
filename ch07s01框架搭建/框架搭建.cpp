#include <SFML/Graphics.hpp>
#include <windows.h>

#define WINDOWS_WIDTH 80
#define WINDOWS_HEIGHT 25
#define STAGE_WIDTH 20
#define STAGE_HEIGHT 20
#define GRIDSIZE 16
#define MAXLENGTH 400

using namespace sf;

bool gameOver;
const int width = STAGE_HEIGHT;
const int height = STAGE_HEIGHT;
int x, y, fruitX, fruitY, score;
int tailX[MAXLENGTH] = { 0 }, tailY[MAXLENGTH] = { 0 };
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

RenderWindow window(VideoMode(width* GRIDSIZE, height* GRIDSIZE), L"Snake by ÖìºÆÑó");
Texture tBackground, tSnack, tFruit;
Sprite spBackground, spSnack, spFruit;

void Initial()
{
	window.setFramerateLimit(10);
	
	tBackground.loadFromFile("../Ì°³ÔÉßËØ²Ädata/images/white.png");
	tSnack.loadFromFile("../Ì°³ÔÉßËØ²Ädata/images/green.png");
	tFruit.loadFromFile("../Ì°³ÔÉßËØ²Ädata/images/red.png");

	spBackground.setTexture(tBackground);
	spSnack.setTexture(tSnack);
	spFruit.setTexture(tFruit);

	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;

	nTail = 1;
}

void Input()
{
	Event event;
	while (window.pollEvent(event))
		if (event.type == Event::Closed)
			window.close();

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		if (dir != RIGHT)
			dir = LEFT;
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		if (dir != LEFT)
			dir = RIGHT;
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		if (dir != DOWN)
			dir = UP;
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		if (dir != UP)
			dir = DOWN;
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		window.clear();
		Sleep(200);
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

void Draw()
{
	window.clear();

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			spBackground.setPosition(i * GRIDSIZE, j * GRIDSIZE);
			window.draw(spBackground);
		}
	}

	for (int i = 0; i < nTail; i++)
	{
		spSnack.setPosition(tailX[i] * GRIDSIZE, tailY[i] * GRIDSIZE);
		window.draw(spSnack);
	}

	spFruit.setPosition(fruitX * GRIDSIZE, fruitY * GRIDSIZE);
	window.draw(spFruit);

	window.display();
}

int main()
{
	Initial();
	while (window.isOpen() && gameOver == false)
	{
		Input();
		Logic();
		Draw();
	}

	return 0;
}