#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "Tetris.h"

#define B_START_CORNER_X 621
#define B_START_CORNER_Y 763
#define B_HOLD_CORNER_X 621
#define B_HOLD_CORNER_Y 822
#define B_LEFT_CORNER_X 70
#define B_LEFT_CORNER_Y 460
#define B_RIGHT_CORNER_X 1295
#define B_RIGHT_CORNER_Y 460
#define P1_SCORE_CORNER_X 366
#define P1_SCORE_CORNER_Y 110
#define P2_SCORE_CORNER_X 1195
#define P2_SCORE_CORNER_Y 110
#define INFO_CORNER_X 562
#define INFO_CORNER_Y 432

using namespace sf;
using namespace std;

typedef enum ButtonState
{
	Continue_Dark,
	Continue_Light,
	Hold_Dark,
	Hold_Light,
	Close_Dark,
	Close_Light,
	Start_Dark,
	Start_Light
};

class Game
{
public:
	Game();
	~Game();

	RenderWindow window;
	Tetris player1, player2;
	bool gameOver, gameQuit;
	Clock clock;
	int Window_width, Window_height, stageWidth, stageHeight;
	bool isGameBegin, isGameHold;
	int imgSetNo;
	int isGameOverState;
	Vector2i mCornPoint;
	int gridsize;
	int imgBGNo, imgSkinNo;
	Texture tTiles, tBackground, tButtons, tSwitcher, tFrame, tCover, tScore, tGameover;
	Sprite sTiles, sBackground, sButtons, sSwitcher, sFrame, sCover, sScore, sGameover;
	IntRect ButtonRectStart, ButtonRectHold, ButtonRectLeft, ButtonRectRight;
	int ButtonState_Start, ButtonState_Hold;
	SoundBuffer sbWin, sbBoom;
	Sound soundWin, soundBoom;
	Music bkMusic;
	Clock gameClock, mouseClickTimer;
	Font font;
	Text text;
	
	void gameInitial();
	void LoadMediaData();
	void gameInput();
	void gameLogic();
	void gameDraw();
	void DrawButton();
	void gameRun();
	void TextOut();
	void DrawResults();
};