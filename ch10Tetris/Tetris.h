#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>

#define GRIDSIZE 35
#define STAGE_WIDTH 10
#define STAGE_HEIGHT 20
#define DELAYVALUE 0.3
#define P1_STAGE_CORNER_X 156
#define P1_STAGE_CORNER_Y 174
#define P2_STAGE_CORNER_X 844
#define P2_STAGE_CORNER_Y 174
#define P1_NEXT_CORNER_X 587
#define P1_NEXT_CORNER_Y 125
#define P2_NEXT_CORNER_X 702
#define P2_NEXT_CORNER_Y 125
#define HOLD_CORNER_X 660
#define HOLD_CORNER_Y 275

using namespace sf;
using namespace std;

typedef enum PLAYROLE
{
	roleNONE,
	rolePLAYER1,
	rolePLAYER2
};

typedef enum gridShape
{
	shapeI,
	shapeS,
	shapeZ,
	shapeT,
	shapeL,
	shapeJ,
	shapeO,
};

class Tetris
{
public:
	Tetris();
	~Tetris();

	Vector2i mCornPoint, nextSquareCornPoint, holdSquareCornPoint;
	int gridsize;
	int imgBGNo, imgSkinNo;
	Texture* tTiles, tBackground, tButtons, tNum, tTimer, tCounter, tGameOver;
	Sprite sTiles, sBackground, sButtons, sNum, sTimer, sCounter, sGameOver;

	int Field[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };
	Vector2i currentSquare[4], nextSquare[4], tempSquare[4], shadowSquare[4];
	int Figures[7][4] =
	{
		3,5,1,7,
		4,5,2,7,
		4,5,3,6,
		5,3,4,7,
		5,3,2,7,
		5,7,3,6,
		2,3,4,5
	};
	int dx;
	bool rotate, hold, hardDrop, newShapeFlag, animationFlag;
	bool gameOver;
	int animationRow[4];
	float animationCtrlValue;
	RenderWindow* window;
	int colorNum, nextcolorNum, tempcolorNum;
	int currentShapeNum, nextShapeNum, tempShapeNum;
	float timer, delay;
	int role;
	int b7Int, b7array[7] = { 0 };
	static int holdcolorNum, holdShapeNum;
	static Vector2i holdSquare[4];
	int score;

	void Initial(Texture* tex);
	void Input(Event* event);
	void Logic();
	void holdFunc();
	void xmove();
	void rotateFunc();
	void ymove();
	void checkLine();
	void Draw(RenderWindow* window);
	bool hitTest();
	bool hitTest(Vector2i hitSquare[]);
	int Bag7(), sBag7();
	void slowLoading();
	void hardDropFunc();
	void shadow();
	void traditionLogic();
	void clearLine();
	void newShapeFunc();
	void animationFunc(int);
	void isWin();
};