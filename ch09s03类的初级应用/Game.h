#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>

#define GRIDSIZE 25
#define LVL1_WIDTH 9
#define LVL1_HEIGHT 9
#define LVL1_NUM 10
#define LVL2_WIDTH 16
#define LVL2_HEIGHT 16
#define LVL2_NUM 40
#define LVL3_WIDTH 30
#define LVL3_HEIGHT 16
#define LVL3_NUM 19

using namespace std;
using namespace sf;

//ö�ٶ�������״̬
typedef enum GRIDSTATE
{
	ncNULL,//�յ�
	ncUNDOWN,//��������
	ncMINE,//����
	ncONE,//����1
	ncTWO,
	ncTHREE,
	ncFOUR,
	ncFIVE,
	ncSIX,
	ncSEVEN,
	ncEIGHT,
	ncFLAG,//���
	ncQ,//�ʺ�
	ncX,//����
	ncBOMBING,//��ը����
	ncUNFOUND//δ����������
};

typedef enum GAMEOVERSTATE
{
	ncNO,//��Ϸδ����
	ncWIN,//��Ϸʤ��
	ncLOSE,//��Ϸʧ��
};

class LEI
{
public:
	int mState;//�׵�״̬
	int mStateBackUp;//����״̬
	bool isPress;//���Ƿ񱻰���
};

class Game
{
public:
	RenderWindow window;

	Game();
	~Game();

	bool gameOver, gameQuit;
	bool mouseDlbClkReady;
	int Window_width, Window_height, stageWidth, stageHeight, mMineNum, mFlagCalc;
	int gamelvl, mTime;//��Ϸ�Ѷȣ���Ϸ��ʱ
	LEI mGameData[LVL3_HEIGHT][LVL3_WIDTH];//����ȡ����Ѷȵ���̨�ߴ�
	bool isGameBegin;
	int isGameOverState;
	Vector2i mCornPoint;//��̨���󶥵�����
	int gridSize;
	int imgBGNo, imgSkinNo;

	Texture tBackground, tTiles, tButtons, tNum, tTimer, tCounter, tGameOver;
	Sprite sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver;
	IntRect ButtonRectEasy, ButtonRectNomal, ButtonRectHard, ButtonRectBG, ButtonRectSkin, ButtonRectRestart, ButtonRectQuit;

	SoundBuffer sbWin, sbBoom;
	Sound soundWin, soundBoom;
	Music bkMusic;

	Clock gameClock, mouseClickTimer;

	void Run();
	void Initial();
	void Input();
	void Logic();
	void Draw();
};

