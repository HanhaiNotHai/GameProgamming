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
#define LVL3_NUM 99

using namespace std;
using namespace sf;

//ö�ٶ�������״̬
typedef enum GRIDSTATE
{
	ncNULL,//�յ� //0
	ncUNDOWN,//��������
	ncMINE,//����
	ncONE,//����1 //3
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

typedef enum MOUSEFUNCTION
{
	RButtonDownFunc,
	LButtonDownFunc,
	LButtonDblClkFunc
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

	bool mouseDblClkReady;
	int gridSize;
	int Window_width, Window_height, stageWidth, stageHeight, mMineNum, mFlagCalc;
	bool gameOver, gameQuit;
	int isGameOverState;
	bool isGameBegin, isMineSetBegin;
	int gamelvl, mTime;//��Ϸ�Ѷȣ���Ϸ��ʱ
	int imgBGNo, imgSkinNo;
	Vector2i mCornPoint;//��̨���󶥵�����
	LEI mGameData[LVL3_HEIGHT][LVL3_WIDTH];//����ȡ����Ѷȵ���̨�ߴ�
	int mouseAction;
	Vector2i mousePoint;
	bool RL_ClkJudge_flag;
	int mouse_RL_ClkReady;
	Vector2i RL_Point;

	Texture tBackground, tTiles, tButtons, tNum, tTimer, tCounter, tGameOver;
	Sprite sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver;
	IntRect ButtonRectEasy, ButtonRectNormal, ButtonRectHard, ButtonRectBG, ButtonRectSkin, ButtonRectRestart, ButtonRectQuit;

	SoundBuffer sbWin, sbBoom;
	Sound soundWin, soundBoom;
	Music bkMusic;

	Clock gameClock, mouseClickTimer;

	void Run();

	void Initial();//��ʼ��
	void IniData();//���ݳ�ʼ��
	void LoadMediaData();//����ý���ز�
	void MineSet(int, int);//���׺���
	void show();

	void Input();
	void RButtonDown(Vector2i);//�һ�
	void LButtonDown(Vector2i);//���
	void LButtonDblClk(Vector2i);//��˫��
	void RL_ButtonDown(Vector2i);
	void RL_ClkJudge();

	void NullClick(int, int);

	void Logic();
	void isWin();
	void undownOpen();
	void unCover();

	void Draw();
	void DrawGrid();
	void DrawButton();
	void DrawScore();
	void DrawTimer();
	void DrawGameEnd();
};