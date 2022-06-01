#include "Game.h"
#include <math.h>

Game::Game()
{
	Window_width = 860;
	Window_height = 600;

	gamelvl = 2;

	window.create(VideoMode(Window_width, Window_height), "MineSweeper by Z");
	//window.create(VideoMode(Window_width, Window_height), "MineSweeper by Z", Style::Close);
}

Game::~Game()
{

}

void Game::Run()
{
	do
	{
		Initial();
		while (window.isOpen() && !gameOver)
		{
			Input();
			Draw();
			Logic();
		}
	} while (window.isOpen() && !gameQuit);
}


void Game::Initial()
{
	window.setFramerateLimit(10);

	gridSize = GRIDSIZE;
	switch (gamelvl)
	{
	case 1:
	{
		stageWidth = LVL1_WIDTH;
		stageHeight = LVL1_HEIGHT;
		mMineNum = LVL1_NUM;
		gridSize = GRIDSIZE * LVL2_WIDTH / LVL1_WIDTH;
		break;
	}
	case 2:
	{
		stageWidth = LVL2_WIDTH;
		stageHeight = LVL2_HEIGHT;
		mMineNum = LVL2_NUM;
		break;
	}
	case 3:
	{
		stageWidth = LVL3_WIDTH;
		stageHeight = LVL3_HEIGHT;
		mMineNum = LVL3_NUM;
		break;
	}
	default:
		break;
	}
	gameOver = false;
	gameQuit = false;
	isGameOverState = ncNO;//游戏结束状态
	mFlagCalc = 0;//棋子数量
	isGameBegin = false;//游戏是否开始
	isMineSetBegin = false;
	mTime = 0;//游戏进行的时间
	imgBGNo = 1;
	imgSkinNo = 1;
	mouse_RL_ClkReady = 0;
	RL_ClkJudge_flag = false;

	mCornPoint.x = (Window_width - stageWidth * gridSize) / 2;
	mCornPoint.y = (Window_height - stageHeight * gridSize) / 2;

	IniData();//初始化素材
	LoadMediaData();//加载素材
	//MineSet(1, 1);
}

void Game::IniData()
{
	int i, j;
	//所有块置为空且未点击
	for (j = 0; j < LVL3_HEIGHT; j++)
		for (i = 0; i < LVL3_WIDTH; i++)
		{
			mGameData[j][i].mState = ncUNDOWN;
			mGameData[j][i].isPress = false;
		}
}

void Game::LoadMediaData()
{
	stringstream ss;

	ss.str("");
	ss << "../MineSweeper_data/images/BK0" << imgBGNo << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "BK image没有找到" << endl;

	ss.str("");
	ss << "../MineSweeper_data/images/Game" << imgSkinNo << ".jpg";
	if (!tTiles.loadFromFile(ss.str()))
		cout << "Game Skin image没有找到" << endl;

	if (!tNum.loadFromFile("../MineSweeper_data/images/num.jpg"))
		cout << "num.jpg没有找到" << endl;
	if (!tTimer.loadFromFile("../MineSweeper_data/images/jishiqi.jpg"))
		cout << "jishiqi.jpg没有找到" << endl;
	if (!tCounter.loadFromFile("../MineSweeper_data/images/jishuqi.jpg"))
		cout << "jishuqi.jpg没有找到" << endl;
	if (!tButtons.loadFromFile("../MineSweeper_data/images/button.jpg"))
		cout << "button.jpg没有找到" << endl;
	if (!tGameOver.loadFromFile("../MineSweeper_data/images/gameover.jpg"))
		cout << "gameover.jpg没有找到" << endl;

	sBackground.setTexture(tBackground);
	sTiles.setTexture(tTiles);
	if (gamelvl == 1)
	{
		float scale = 1.0 * LVL2_WIDTH / LVL1_WIDTH;
		sTiles.setScale(scale, scale);
	}
	else
		sTiles.setScale(1.0, 1.0);
	sNum.setTexture(tNum);
	sTimer.setTexture(tTimer);
	sCounter.setTexture(tCounter);
	sButtons.setTexture(tButtons);
	sGameOver.setTexture(tGameOver);
}

void Game::MineSet(int Px, int Py)//布雷
{
	int mCount, i, j, k, l;
	mCount = 0;
	srand(time(NULL));//用当前系统时间作为随机数生成器的种子
	//随机布雷

	do
	{
		bool flag = true;//当前循环是否布雷的判定变量
		k = rand() % stageHeight;//生成随机数
		l = rand() % stageWidth;
		for (i = Py - 1; i < Py + 2; i++)//鼠标第一下点击处及周围8领域
			for (j = Px - 1; j < Px + 2; j++)
				if (i >= 0 && i < stageHeight && j >= 0 && j < stageWidth)
					if (k == i && l == j)
						flag = false;//随机坐标若处于9宫格覆盖范围，则不要布雷
		if (flag && mGameData[k][l].mState == ncUNDOWN)
		{
			mGameData[k][l].mState = ncMINE;
			mGameData[k][l].mStateBackUp = ncMINE;//备份状态
			mCount++;
		}
	} while (mCount < mMineNum);

	show();

	//方格赋值
	for (i = 0; i < stageHeight; i++)
	{
		for (j = 0; j < stageWidth; j++)
		{
			if (mGameData[i][j].mState != ncMINE)
			{
				mCount = 0;
				//计算i,j周围雷的数目
				for (k = i - 1; k < i + 2; k++)
				{
					for (l = j - 1; l < j + 2; l++)
					{
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncMINE)
								mCount++;
						}
					}
				}
				//保存状态
				if (mCount == 0)
					mGameData[i][j].mState = mGameData[i][j].mStateBackUp = ncNULL;
				else
					mGameData[i][j].mState = mGameData[i][j].mStateBackUp = mCount + 2;
			}
		}
	}
	show();
}

void Game::show()
{
	int i, j;
	for (i = 0; i < stageHeight; i++)
	{
		for (j = 0; j < stageWidth; j++)
		{
			//if (mGameData[i][j].mState == ncMINE)
			//	cout << "M";
			//else if (ncONE <= mGameData[i][j].mState && mGameData[i][j].mState <= ncEIGHT)
			//	cout << mGameData[i][j].mState - 2;
			//else
			//	cout << "0";
			cout << mGameData[i][j].mState;
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}


void Game::Input()
{
	Vector2u size = window.getSize();
	Vector2f scale(1.0 * Window_width / size.x, 1.0 * Window_height / size.y);
	Vector2i mPoint(Mouse::getPosition(window).x * scale.x, Mouse::getPosition(window).y * scale.y);
	Vector2i P1, P2;

	Event event;
	while (window.pollEvent(event))
		switch (event.type)
		{
		case Event::Closed:
		{
			cout << "Closed" << endl;
			window.close();
			gameQuit = true;
			cout << endl;
			break;
		}
		case Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case Keyboard::A:
			{
				cout << "A";
				RL_ButtonDown(Mouse::getPosition(window));
				break;
			}
			default:
				break;
			}
			break;
		}
		case Event::EventType::KeyReleased:
		{
			switch (event.key.code)
			{
			case Keyboard::Escape:
			{
				cout << "Escape Closed" << endl;
				window.close();
				gameQuit = true;
				cout << endl;
				break;
			}
			case Keyboard::R:
			{
				cout << "R Restart" << endl;
				Initial();
				break;
			}
			case Keyboard::Q:
			{
				show();
				break;
			}
			default:
				break;
			}
			break;
		}
		case Event::MouseButtonPressed:
		{
			P2 = Mouse::getPosition(window);
			cout << "P2 " << P2.x << "," << P2.y << endl;
			switch (event.mouseButton.button)
			{
			case Mouse::Left:
			{
				cout << "Left Pressed" << endl;
				if (isGameOverState == ncNO)
				{
					if (mouseClickTimer.getElapsedTime().asMilliseconds() > 300)
					{
						mouseClickTimer.restart();
						if (Mouse::isButtonPressed(Mouse::Right))
						{
							cout << "RL_ButtonDown" << endl;
							RL_ButtonDown(P2);
						}
						else
						{
							cout << "LButtonDown" << endl;
							LButtonDown(P2);
							mouseDblClkReady = true;
							cout << "mouseDblClkReady = true" << endl;
						}
					}
					mouse_RL_ClkReady++;
					cout << "mouse_RL_ClkReady = " << mouse_RL_ClkReady << endl;
				}
				break;
			}
			case Mouse::Right:
			{
				cout << "Right Pressed" << endl;
				if (isGameOverState == ncNO)
				{
					mouse_RL_ClkReady++;
					cout << "mouse_RL_ClkReady = " << mouse_RL_ClkReady << endl;
					if (mouse_RL_ClkReady == 2)
					{
						cout << "RL_ButtonDown" << endl;
						RL_ButtonDown(P2);
					}
					else
					{
						cout << "RButtonDown" << endl;
						RButtonDown(P2);
					}
				}
				break;
			}
			default:
				break;
			}
			cout << endl;
			break;
		}
		case Event::MouseButtonReleased:
		{
			switch (event.mouseButton.button)
			{
			case Mouse::Left:
			{
				cout << "Left Releasesd" << endl;
				P1 = Mouse::getPosition(window);
				cout << "P1 " << P1.x << "," << P1.y << endl;
				if (isGameOverState == ncNO)
				{
					if (mouseDblClkReady)
					{
						mouseDblClkReady = false;
						cout << "mouseDblClkReady = false" << endl;
					}
					else
					{
						if (mouseClickTimer.getElapsedTime().asMilliseconds() < 300/* && abs(P2.x - P1.x) < gridSize / 4 && abs(P2.y - P1.y) < gridSize / 4*/)
						{
							cout << "LButtonDblClk" << endl;
							LButtonDblClk(P1);
						}
					}
					mouse_RL_ClkReady = 0;
					cout << "mouse_RL_ClkReady = " << mouse_RL_ClkReady << endl;
					mouseClickTimer.restart();

					if (isGameBegin == false)
					{
						if (ButtonRectEasy.contains(P1))
						{
							cout << "ButtonRectEast" << endl;
							gamelvl = 1;
							Initial();
						}
						if (ButtonRectNormal.contains(event.mouseButton.x, event.mouseButton.y))
						{
							cout << "ButtonRectNormal" << endl;
							gamelvl = 2;
							Initial();
						}
						if (ButtonRectHard.contains(event.mouseButton.x, event.mouseButton.y))
						{
							cout << "ButtonRectEast" << endl;
							gamelvl = 3;
							Initial();
						}
						//Initial();
					}
				}
				if (ButtonRectBG.contains(mPoint.x, mPoint.y))
				{
					cout << "ButtonRectBG" << endl;
					imgBGNo = imgBGNo % 7 + 1;
					LoadMediaData();
				}
				if (ButtonRectSkin.contains(mPoint.x, mPoint.y))
				{
					cout << "ButtonRectSkin" << endl;
					imgSkinNo = imgSkinNo % 6 + 1;
					LoadMediaData();
				}
				if (ButtonRectRestart.contains(mPoint.x, mPoint.y))
				{
					cout << "ButtonRectRestart" << endl;
					Initial();
				}
				if (ButtonRectQuit.contains(mPoint.x, mPoint.y))
				{
					cout << "ButtonRectQuit" << endl;
					window.close();
					gameQuit = true;
				}
				break;
			}
			case Mouse::Right:
			{
				cout << "right released" << endl;
				mouse_RL_ClkReady = 0;
				cout << "mouse_RL_ClkReady = " << mouse_RL_ClkReady << endl;
				break;
			}
			default:
				break;
			}
			cout << endl;
			break;
		}
		default:
			break;
		}
}

void Game::RButtonDown(Vector2i mPoint)
{
	int i, j;
	i = (mPoint.y - mCornPoint.y) / gridSize;
	j = (mPoint.x - mCornPoint.x) / gridSize;

	if (0 <= i && i < stageHeight && 0 <= j && j < stageWidth)
	{
		if (isGameBegin == false)
		{
			isGameBegin = true;
			gameClock.restart();
		}
		if (mGameData[i][j].isPress == false)
		{
			mGameData[i][j].isPress = true;
			mouseClickTimer.restart();
			mGameData[i][j].mStateBackUp = mGameData[i][j].mState;
			mGameData[i][j].mState = ncFLAG;
			mFlagCalc++;
		}
		else
		{
			if (mGameData[i][j].mState == ncFLAG)
			{
				mGameData[i][j].isPress = true;
				mGameData[i][j].mState = ncQ;
				mFlagCalc--;
			}
			else if (mGameData[i][j].mState == ncQ)
			{
				mGameData[i][j].isPress = false;
				mGameData[i][j].mState = mGameData[i][j].mStateBackUp;
			}
		}
	}
}

void Game::LButtonDown(Vector2i mPoint)
{
	int i, j;
	i = (mPoint.y - mCornPoint.y) / gridSize;
	j = (mPoint.x - mCornPoint.x) / gridSize;

	if (0 <= i && i < stageHeight && 0 <= j && j < stageWidth)
	{
		if (isGameBegin == false)
		{
			isGameBegin = true;
			gameClock.restart();
		}
		if (isMineSetBegin == false)
		{
			isMineSetBegin = true;
			MineSet(i, j);
		}
		if (mGameData[i][j].mState != ncFLAG)
		{
			if (mGameData[i][j].isPress == false)
			{
				mGameData[i][j].isPress = true;
				if (mGameData[i][j].mState == ncMINE)
				{
					isGameBegin = false;
					isGameOverState = ncLOSE;
					mGameData[i][j].mState = ncBOMBING;
					unCover();
				}
				if (mGameData[i][j].mState == ncNULL)
					NullClick(i, j);
			}
		}
	}
}

void Game::NullClick(int i, int j)
{
	int k, l;
	for (k = i - 1; k < i + 2; k++)
		for (l = j - 1; l < j + 2; l++)
			if (0 <= k && k < stageHeight && 0 <= l && l < stageWidth)
			{
				if (mGameData[k][l].isPress == false)
				{
					mGameData[k][l].isPress = true;
					if (mGameData[k][l].mState == ncNULL)
						NullClick(k, l);
				}
			}
}

void Game::LButtonDblClk(Vector2i mPoint)
{
	int i, j, k, l, lvl;
	i = (mPoint.y - mCornPoint.y) / gridSize;
	j = (mPoint.x - mCornPoint.x) / gridSize;

	if (0 <= i && i < stageHeight && 0 <= j && j < stageWidth)
	{
		if (mGameData[i][j].isPress == true)
		{
			if (mGameData[i][j].mState != ncFLAG)
			{
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
					{
						if (0 <= k && k < stageHeight && 0 <= l && l < stageHeight)
						{
							if (mGameData[k][l].mState == ncFLAG)
							{
								if (mGameData[k][l].mStateBackUp != ncMINE)
								{
									isGameOverState == ncLOSE;
									isGameBegin = false;
									unCover();
								}
							}
							else
							{
								if (mGameData[k][l].isPress == false)
								{
									mGameData[k][l].isPress = true;
									if (mGameData[k][l].mState == ncMINE)
									{
										isGameOverState == ncLOSE;
										isGameBegin = false;
										mGameData[k][l].mState = ncBOMBING;
										unCover();
									}
									if (mGameData[k][l].mState == ncNULL)
										NullClick(k, l);
								}
							}
						}
					}
			}
		}
	}
}

void Game::RL_ButtonDown(Vector2i mPoint)
{
	int i, j, k, l;
	i = (mPoint.y - mCornPoint.y) / gridSize;
	j = (mPoint.x - mCornPoint.x) / gridSize;

	if (0 <= i && i < stageHeight && 0 <= j && j < stageHeight)
	{
		if (mGameData[i][j].isPress == true)
		{
			if (mGameData[i][j].mState != ncFLAG && mGameData[i][j].mState != ncQ)
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
						if (0 <= k && k < stageHeight && 0 <= l && stageWidth)
							if (mGameData[k][l].isPress == false)
							{
								mGameData[k][l].isPress = true;
								mGameData[k][l].mState = ncX;
							}
		}
		else
		{
			for (k = i - 1; k < i + 2; k++)
				for (l = j - 1; l < j + 2; l++)
					if (0 <= k && k < stageHeight && 0 <= l && l < stageWidth)
						if (mGameData[k][l].isPress == false)
						{
							mGameData[k][l].isPress = true;
							mGameData[k][l].mState = ncX;
						}
			mGameData[i][j].isPress = false;
		}
	}
	RL_Point = mPoint;
	RL_ClkJudge_flag = true;
}

void Game::RL_ClkJudge()
{
	int i, j, k, l, mineNum = 0, flagNum = 0;
	i = (RL_Point.y - mCornPoint.y) / gridSize;
	j = (RL_Point.x - mCornPoint.x) / gridSize;

	if (0 <= i && i < stageHeight && 0 <= j && j < stageWidth)
	{
		if (mGameData[i][j].isPress == true)
		{
			if (mGameData[i][j].mState != ncFLAG)
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
						if (0 <= k && k < stageHeight && 0 <= l && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncFLAG)
								flagNum++;
							if (mGameData[k][l].mState == ncX)
							{
								mGameData[k][l].isPress = false;
								mGameData[k][l].mState = mGameData[k][l].mStateBackUp;
							}
						}
		}
		else
		{
			if (mGameData[i][j].mState != ncFLAG)
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
						if (0 <= k && k < stageHeight && 0 <= l && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncX)
							{
								mGameData[k][l].isPress = false;
								mGameData[k][l].mState = mGameData[k][l].mStateBackUp;
							}
						}
		}
		if (mGameData[i][j].mState == flagNum + 2)
			LButtonDblClk(RL_Point);
	}
	RL_ClkJudge_flag = false;
}


void Game::Logic()
{
	if (mouse_RL_ClkReady == 0 && RL_ClkJudge_flag == true)
		RL_ClkJudge();
	isWin();
}

void Game::isWin()
{
	int i, j, c = 0;
	if (isGameOverState != ncLOSE)
	{
		for (i = 0; i < stageHeight; i++)
			for (j = 0; j < stageWidth; j++)
				if (mGameData[i][j].isPress == false || mGameData[i][j].mState == ncFLAG)
					c++;
	}
	if (c == mMineNum)
	{
		isGameBegin = false;
		mFlagCalc = mMineNum;
		undownOpen();
		isGameOverState = ncWIN;
	}
}

void Game::undownOpen()
{
	int i, j;
	for (i = 0; i < stageHeight; i++)
		for (j = 0; j < stageWidth; j++)
			if (mGameData[i][j].isPress == false)
			{
				mGameData[i][j].isPress = true;
				if (mGameData[i][j].mState == ncMINE)
					mGameData[i][j].mState = ncFLAG;
			}
}

void Game::unCover()
{
	int i, j;
	for (i = 0; i < stageHeight; i++)
		for (j = 0; j < stageWidth; j++)
			if (mGameData[i][j].isPress == false)
				if (mGameData[i][j].mState == ncMINE)
				{
					mGameData[i][j].isPress = true;
					mGameData[i][j].mState = ncUNFOUND;
				}
}


void Game::Draw()
{
	//Vector2u size(Window_width, Window_height);
	//window.setSize(size);

	window.clear();

	sBackground.setPosition(0, 0);
	window.draw(sBackground);

	DrawGrid();
	DrawButton();
	DrawScore();
	DrawTimer();
	if (isGameOverState)
		DrawGameEnd();

	window.display();
}

void Game::DrawGrid()
{
	for (int j = 0; j < stageHeight; j++)
		for (int i = 0; i < stageWidth; i++)
		{
			if (mGameData[j][i].isPress)
				sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			else
				sTiles.setTextureRect(IntRect(ncUNDOWN * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(mCornPoint.x + i * gridSize, mCornPoint.y + j * gridSize);
			window.draw(sTiles);
		}
}

void Game::DrawButton()
{
	Vector2i LeftCorner;
	int ButtonWidth = 60;
	int ButtonHeight = 36;
	int detaX = (Window_width - ButtonWidth * 7) / 8;
	LeftCorner.y = Window_height - GRIDSIZE * 3;

	LeftCorner.x = detaX;
	sButtons.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectEasy.left = LeftCorner.x;
	ButtonRectEasy.top = LeftCorner.y;
	ButtonRectEasy.width = ButtonWidth;
	ButtonRectEasy.height = ButtonHeight;
	window.draw(sButtons);

	LeftCorner.x = detaX * 2 + ButtonWidth;
	sButtons.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectNormal.left = LeftCorner.x;
	ButtonRectNormal.top = LeftCorner.y;
	ButtonRectNormal.width = ButtonWidth;
	ButtonRectNormal.height = ButtonHeight;
	window.draw(sButtons);

	LeftCorner.x = detaX * 3 + ButtonWidth * 2;
	sButtons.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectHard.left = LeftCorner.x;
	ButtonRectHard.top = LeftCorner.y;
	ButtonRectHard.width = ButtonWidth;
	ButtonRectHard.height = ButtonHeight;
	window.draw(sButtons);

	LeftCorner.x = detaX * 4 + ButtonWidth * 3;
	sButtons.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectSkin.left = LeftCorner.x;
	ButtonRectSkin.top = LeftCorner.y;
	ButtonRectSkin.width = ButtonWidth;
	ButtonRectSkin.height = ButtonHeight;
	window.draw(sButtons);

	LeftCorner.x = detaX * 5 + ButtonWidth * 4;
	sButtons.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectBG.left = LeftCorner.x;
	ButtonRectBG.top = LeftCorner.y;
	ButtonRectBG.width = ButtonWidth;
	ButtonRectBG.height = ButtonHeight;
	window.draw(sButtons);

	LeftCorner.x = detaX * 6 + ButtonWidth * 5;
	sButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectRestart.left = LeftCorner.x;
	ButtonRectRestart.top = LeftCorner.y;
	ButtonRectRestart.width = ButtonWidth;
	ButtonRectRestart.height = ButtonHeight;
	window.draw(sButtons);

	LeftCorner.x = detaX * 7 + ButtonWidth * 6;
	sButtons.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);
	ButtonRectQuit.left = LeftCorner.x;
	ButtonRectQuit.top = LeftCorner.y;
	ButtonRectQuit.width = ButtonWidth;
	ButtonRectQuit.height = ButtonHeight;
	window.draw(sButtons);
}

void Game::DrawScore()
{
	Vector2i LeftCorner;
	LeftCorner.x = Window_width - sCounter.getLocalBounds().width * 1.25;
	LeftCorner.y = sCounter.getLocalBounds().height * 0.5;
	sCounter.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sCounter);

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sCounter.getLocalBounds().width - NumSize;
	LeftCorner.y = LeftCorner.y + sCounter.getLocalBounds().height * 0.5 - NumSize * 0.5;

	int mScore = mMineNum - mFlagCalc;

	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);

	a = mScore / 10 % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);

	a = mScore / 100 % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);
}

void Game::DrawTimer()
{
	Vector2i LeftCorner;
	LeftCorner.x = sTimer.getLocalBounds().width * 0, 25;
	LeftCorner.y = sTimer.getLocalBounds().height * 0, 25;
	sTimer.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sTimer);
	if (isGameBegin)
		mTime = gameClock.getElapsedTime().asSeconds();
	int mScore = mTime;
	if (mScore > 999)
		mScore = 999;
	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sTimer.getLocalBounds().width - NumSize * 1.5;
	LeftCorner.y = LeftCorner.y + sTimer.getLocalBounds().height * 0.5 - NumSize * 0.5;

	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);

	a = mScore / 10 % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);

	a = mScore / 100 % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);
}

void Game::DrawGameEnd()
{
	Vector2i LeftCorner;
	int ButtonWidth = 200;
	int ButtonHeight = sGameOver.getLocalBounds().height;
	LeftCorner.x = (Window_width - ButtonWidth) / 2;
	LeftCorner.y = (Window_height - ButtonHeight) / 2;
	sGameOver.setPosition(LeftCorner.x, LeftCorner.y);

	if (isGameOverState == ncWIN)
		sGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	else if (isGameOverState == ncLOSE)
		sGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));

	window.draw(sGameOver);
}