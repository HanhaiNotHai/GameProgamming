#include "Tetris.h"

Tetris::Tetris()
{
	dx = 0;
	rotate = false;
	colorNum = 1;
	timer = 0;
	delay = 0.3;
}

Tetris::~Tetris()
{

}

void Tetris::Initial()
{

}

void Tetris::Input()
{

}

void Tetris::Logic()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].x += dx;
	}

	if (!hitTest())
		for (int i = 0; i < 4; i++)
			currentSquare[i] = tempSquare[i];

	if (rotate)
	{
		Vector2i p = currentSquare[1];
		for (int i = 0; i < 4; i++)
		{
			int x = currentSquare[i].y - p.y;
			int y = currentSquare[i].x - p.x;
			currentSquare[i].x = p.x - x;
			currentSquare[i].y = p.y - y;
		}

		if (!hitTest())
			for (int i = 0; i < 4; i++)
				currentSquare[i] = tempSquare[i];
	}

	if (timer > delay)
	{
		for (int i = 0; i < 4; i++)
		{
			tempSquare[i] = currentSquare[i];
			currentSquare[i].y += 1;
		}

		if (!hitTest())
		{
			for (int i = 0; i < 4; i++)
				Field[tempSquare[i].y][tempSquare[i].x] = colorNum;

			colorNum = 1 + rand() % 7;
			int n = rand() % 7;
			for (int i = 0; i < 4; i++)
			{
				currentSquare[i].x = Figures[n][i] % 2;
				currentSquare[i].y = Figures[n][i] / 2;
			}
		}
		timer = 0;
	}

	int k = STAGE_HEIGHT - 1;
	for (int i = STAGE_HEIGHT - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j])
				count++;
			Field[k][j] = Field[i][j];
		}
		if (count < STAGE_WIDTH)
			k--;
	}
	rotate = false;
}

void Tetris::Draw()
{

}

bool Tetris::hitTest()
{
	for (int i = 0; i < 4; i++)
		if (currentSquare[i].x < 0 || currentSquare[i].x >= STAGE_WIDTH || currentSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (Field[currentSquare[i].y][currentSquare[i].x])
			return false;
	return true;
}