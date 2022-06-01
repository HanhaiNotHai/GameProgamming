#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>

#define GRIDSIZE 35
#define STAGE_WIDTH 10
#define STAGE_HEIGHT 20

using namespace sf;

class Tetris
{
public:
	Tetris();
	~Tetris();

	Vector2i mCornPoint;
	int gridsize;
	int imgBGNo, imgSkinNo;
	Texture* tTiles;
	Texture tBackground, tButtons, tNum, tTimer, tCounter, tGameOver;
	Sprite sTiles, sBackground, sButtons, sNum, sTimer, sCounter, sGameOver;

	int Field[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };
	Vector2i currentSquare[4], nextSquare[4], holdSquare[4], tempSquare[4];
	int Figures[7][4] =
	{
		1,3,5,7,
		2,4,5,7,
		3,4,5,6,
		3,4,5,7,
		2,3,5,7,
		3,5,6,7,
		2,3,4,5
	};
	int dx;
	bool rotate;
	int colorNum;
	float timer, delay;

	void Initial();
	void Input();
	void Logic();
	void Draw();
	bool hitTest();
};

