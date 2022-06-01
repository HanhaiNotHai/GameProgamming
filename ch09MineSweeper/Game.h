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

//枚举定义网格状态
typedef enum GRIDSTATE
{
	ncNULL,//空地 //0
	ncUNDOWN,//背景方块
	ncMINE,//地雷
	ncONE,//数字1 //3
	ncTWO,
	ncTHREE,
	ncFOUR,
	ncFIVE,
	ncSIX,
	ncSEVEN,
	ncEIGHT,
	ncFLAG,//标记
	ncQ,//问号
	ncX,//备用
	ncBOMBING,//爆炸的雷
	ncUNFOUND//未检测出来的雷
};

typedef enum GAMEOVERSTATE
{
	ncNO,//游戏未结束
	ncWIN,//游戏胜利
	ncLOSE,//游戏失败
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
	int mState;//雷的状态
	int mStateBackUp;//备份状态
	bool isPress;//雷是否被按下
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
	int gamelvl, mTime;//游戏难度，游戏计时
	int imgBGNo, imgSkinNo;
	Vector2i mCornPoint;//舞台的左顶点坐标
	LEI mGameData[LVL3_HEIGHT][LVL3_WIDTH];//数组取最高难度的舞台尺寸
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

	void Initial();//初始化
	void IniData();//数据初始化
	void LoadMediaData();//加载媒体素材
	void MineSet(int, int);//布雷函数
	void show();

	void Input();
	void RButtonDown(Vector2i);//右击
	void LButtonDown(Vector2i);//左击
	void LButtonDblClk(Vector2i);//左双击
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