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
#define STEP 0.1

using namespace sf;

bool gameOver;
int delay;
int GameMode;
float stepX, stepY;
const int width = STAGE_HEIGHT;
const int height = STAGE_HEIGHT;
int x, y, fruitX, fruitY, score;
int tailX[MAXLENGTH] = { 0 }, tailY[MAXLENGTH] = { 0 };
int nTail, headRotation;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, dir_ing;
bool updateFlag;

RenderWindow window(VideoMode((width + 2)* GRIDSIZE + INFO_WIDTH, (height + 2)* GRIDSIZE), L"Snake by ??????");
Texture tBackground, tSnakeHead, tSnakeBody, tFruit;
Sprite spBackground, spSnakeHead, spSnakeBody, spFruit;
Font font;
Text text;

void Initial()
{
	window.setFramerateLimit(60);

	gameOver = false;
	delay = 0;
	GameMode = 1;
	stepX = 0.0;
	stepY = 0.0;
	dir = STOP;
	dir_ing = STOP;
	updateFlag = false;
	tailX[0] = x = width / 2;
	tailY[0] = y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;

	nTail = 1;
	headRotation = 0;

	if (!tBackground.loadFromFile("../̰?????ز?data/images/BK.png"))
		std::cout << "BK.pngû???ҵ?" << std::endl;
	if (!tSnakeHead.loadFromFile("../̰?????ز?data/images/sh01.png"))
		std::cout << "sh01.pngû???ҵ?" << std::endl;
	if (!tSnakeBody.loadFromFile("../̰?????ز?data/images/sb0102.png"))
		std::cout << "sb0102.pngû???ҵ?" << std::endl;
	if (!tFruit.loadFromFile("../̰?????ز?data/images/sb0202.png"))
		std::cout << "sb0202.pngû???ҵ?" << std::endl;

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
		std::cout << "????û???ҵ?" << std::endl;

	text.setFont(font);
}

void Prompt_info(int _x, int _y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 24;
	text.setCharacterSize(24);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"????Ϸ˵????");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	A.??????ײ????Ϸ????");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	B.?߿ɴ?ǽ");
	window.draw(text);
	initialY += CharacterSize * 2;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"??????˵????");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	???????ƶ?????A");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	???????ƶ?????D");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	???????ƶ?????W");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	???????ƶ?????S");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	????ʼ??Ϸ?????ⷽ????");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	???˳???Ϸ??x");
	window.draw(text);
	initialY += CharacterSize;

	/*text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	????ͣ???ո?");
	window.draw(text);
	initialY += CharacterSize;*/

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"	??????ģʽ?л???Space");
	window.draw(text);
	initialY += CharacterSize * 2;

	text.setPosition(_x + initialX, _y + initialY);
	text.setString(L"?????ߣ???????ý ??????");
	window.draw(text);
}

void Input()
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		if (event.type == Event::EventType::KeyReleased && event.key.code == Keyboard::Space)
		{
			GameMode = 3 - GameMode;
			std::cout << "GameMode changed!" << std::endl;
		}
	}

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

	//???Ʊ???
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			spBackground.setPosition(i * GRIDSIZE + deltaX, j * GRIDSIZE + deltaY);
			window.draw(spBackground);
		}
	}

	//??????
	spSnakeHead.setPosition(tailX[0] * GRIDSIZE + deltaX, tailY[0] * GRIDSIZE + deltaY);
	spSnakeHead.setRotation(headRotation);
	window.draw(spSnakeHead);
	for (int i = 1; i < nTail; i++)
	{
		spSnakeBody.setPosition(tailX[i] * GRIDSIZE + deltaX, tailY[i] * GRIDSIZE + deltaY);
		window.draw(spSnakeBody);
	}

	//????ˮ??
	spFruit.setPosition(fruitX * GRIDSIZE + deltaX, fruitY * GRIDSIZE + deltaY);
	window.draw(spFruit);

	window.display();
}

void LogicStep()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;

	bool updateFlag = false;//ȷ???ߴ?????ȫ???????߼??ж?

	tailX[0] = x;
	tailY[0] = y;

	switch (dir_ing)
	{
	case LEFT:
		stepX -= STEP;
		if (stepX < -0.9999 || stepX>0.9999) {
			x--;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = -90;
			updateFlag = true;
		}
		break;
	case RIGHT:
		stepX += STEP;
		if (stepX < -0.9999 || stepX>0.9999) {
			x++;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = 90;
			updateFlag = true;
		}
		break;
	case UP:
		stepY -= STEP;
		if (stepY < -0.9999 || stepY>0.9999) {
			y--;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = 0;
			updateFlag = true;
		}
		break;
	case DOWN:
		stepY += STEP;
		if (stepY < -0.9999 || stepY>0.9999) {
			y++;
			stepX = 0;
			stepY = 0;
			dir_ing = dir;
			headRotation = 0;
			updateFlag = true;
		}
		break;
	default:
		dir_ing = dir;
		break;
	}
	dir_ing = dir;

	if (x >= width)
		x = 0;
	else if (x < 0)
		x = width - 1;
	if (y >= height)
		y = 0;
	else if (y < 0)
		y = height - 1;

	tailX[0] = x;
	tailY[0] = y;
	if (updateFlag == true) {
		if (x == fruitX && y == fruitY)
		{
			score += 10;
			//soundEat.play();
			fruitX = rand() % width;
			fruitY = rand() % height;
			nTail++;
		}


		for (int i = 1; i < nTail; i++)
		{
			prev2X = tailX[i];
			prev2Y = tailY[i];
			tailX[i] = prevX;
			tailY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}


		for (int i = 1; i < nTail; i++)
		{
			if (tailX[i] == x && tailY[i] == y)
			{
				//soundDie.play();
				gameOver = true;
			}
		}
	}
}

void DrawStep()
{
	window.clear(Color::Color(251, 188, 5, 255));//????
	Prompt_info(width * GRIDSIZE + GRIDSIZE, GRIDSIZE);

	int detaX = GRIDSIZE / SCALE / 2;//??Ϸ??̨?󶥵????Դ??????Ͻǵ?ˮƽ????ֱ??????ƫ????
	int detaY = GRIDSIZE / SCALE / 2;
	//???Ʊ???
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			spBackground.setPosition(i * GRIDSIZE + detaX, j * GRIDSIZE + detaY);
			window.draw(spBackground);
		}
	//??????
	float stepLength;
	stepLength = stepX + stepY;
	if (stepLength < 0)
		stepLength = -stepLength;

	spSnakeHead.setPosition((tailX[0] + stepX) * GRIDSIZE + detaX, (tailY[0] + stepY) * GRIDSIZE + detaY);
	spSnakeHead.setRotation(headRotation);
	window.draw(spSnakeHead);

	for (int i = 1; i < nTail; i++)
	{
		//if (i % 2 == 1) {
		if (tailY[i] == tailY[i - 1] && tailX[i] != tailX[i - 1])
			spSnakeBody.setPosition((tailX[i] + (tailX[i - 1] - tailX[i]) * stepLength) * GRIDSIZE + detaX, tailY[i] * GRIDSIZE + detaY);
		if (tailY[i] != tailY[i - 1] && tailX[i] == tailX[i - 1])
			spSnakeBody.setPosition(tailX[i] * GRIDSIZE + detaX, (tailY[i] + (tailY[i - 1] - tailY[i]) * stepLength) * GRIDSIZE + detaY);
		window.draw(spSnakeBody);
		/*}
		else {
			if (tailY[i] == tailY[i - 1] && tailX[i] != tailX[i - 1])
				spSnakeBody.setPosition((tailX[i] + (tailX[i - 1] - tailX[i]) * stepLength) * GRIDSIZE + detaX, tailY[i] * GRIDSIZE + detaY);
			if (tailY[i] != tailY[i - 1] && tailX[i] == tailX[i - 1])
				spSnakeBody.setPosition(tailX[i] * GRIDSIZE + detaX, (tailY[i] + (tailY[i - 1] - tailY[i]) * stepLength) * GRIDSIZE + detaY);
			window.draw(spSnakeBody);
		}*/
	}

	//????ˮ??
	spFruit.setPosition(fruitX * GRIDSIZE + detaX, fruitY * GRIDSIZE + detaY);
	window.draw(spFruit);

	/*if (gameOver) {
		gameOver_info(width / 8 * GRIDSIZE, height / 4 * GRIDSIZE);
	}*/

	window.display();
}

int main()
{
	Initial();
	while (window.isOpen() && gameOver == false)
	{
		Input();
		switch (GameMode)
		{
		case 1:
		{
			delay++;
			if (delay % 10 == 0)
			{
				delay = 0;
				Logic();
			}
			Draw();
			break;
		}
		case 2:
		{
			LogicStep();
			DrawStep();
			break;
		}
		default:
			break;
		}
	}

	return 0;
}