#include "Game.h"

Game::Game()
{
	Window_width = 1350;
	Window_height = 1000;
	imgBGNo = 1;
	imgSkinNo = 1;
	imgSetNo = 1;
	ButtonState_Start = Start_Dark;
	window.create(VideoMode(Window_width, Window_height), "Tetris");
}

Game::~Game() {}

void Game::gameInitial()
{
	window.setFramerateLimit(15);
	LoadMediaData();

	isGameBegin = false;
	isGameHold = false;
	gameOver = false;

	ButtonState_Hold = Hold_Dark;

	player1.role = rolePLAYER1;
	player2.role = rolePLAYER2;
	player1.Initial(&tTiles);
	player2.Initial(&tTiles);
}

void Game::LoadMediaData()
{
	stringstream ss;

	ss.str("");
	ss << "../Tetris_data/images/bg" << imgSetNo << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "BK imageû���ҵ�" << endl;

	ss.str("");
	ss << "../Tetris_data/images/tiles" << imgSetNo << ".jpg";
	if (!tTiles.loadFromFile(ss.str()))
		cout << "tiles imageû���ҵ�" << endl;

	if (!tFrame.loadFromFile("../Tetris_data/images/frame.png"))
		cout << "frame.pngû���ҵ�" << endl;
	if (!tCover.loadFromFile("../Tetris_data/images/cover.png"))
		cout << "cover.pngû���ҵ�" << endl;
	if (!tGameover.loadFromFile("../Tetris_data/images/end.png"))
		cout << "end.pngû���ҵ�" << endl;
	if (!tButtons.loadFromFile("../Tetris_data/images/button.png"))
		cout << "button.pngû���ҵ�" << endl;
	if (!tSwitcher.loadFromFile("../Tetris_data/images/bgSwitch.png"))
		cout << "button.pngû���ҵ�" << endl;

	sBackground.setTexture(tBackground);
	//sTiles.setTexture(tTiles);
	sFrame.setTexture(tFrame);
	sCover.setTexture(tCover);
	sGameover.setTexture(tGameover);
	sButtons.setTexture(tButtons);
	sSwitcher.setTexture(tSwitcher);
	sSwitcher.setOrigin(sSwitcher.getLocalBounds().width / 2.0, sSwitcher.getLocalBounds().height / 2.0);

	if (!font.loadFromFile("../Tetris_data/Fonts/simkai.ttf"))
		cout << "simkai.ttfû���ҵ�" << endl;
	text.setFont(font);
}

void Game::gameInput()
{
	Event event;
	window.setKeyRepeatEnabled(false);
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case Event::Closed:
		{
			window.close();
			gameQuit = true;
			break;
		}
		case Event::KeyPressed:
		{
			break;
		}
		case Event::KeyReleased:
		{
			switch (event.key.code)
			{
			case Keyboard::Escape:
			{
				window.close();
				gameQuit = true;
				break;
			}
			case Keyboard::R:
			{
				gameOver = true;
				break;
			}
			default:
				break;
			}
			break;
		}
		case Event::MouseButtonReleased:
		{
			if (ButtonRectStart.contains(event.mouseButton.x, event.mouseButton.y))
			{
				if (isGameBegin)
					ButtonState_Start = Start_Light;
				else
				{
					ButtonState_Start = Close_Light;
					player1.animationFlag = false;
					player2.animationFlag = false;
				}
				isGameBegin = !isGameBegin;
			}
			if (ButtonRectHold.contains(event.mouseButton.x, event.mouseButton.y))
			{
				if (isGameHold)
					ButtonState_Hold = Hold_Light;
				else
					ButtonState_Hold = Continue_Light;
				isGameHold = !isGameHold;
			}
			if (ButtonRectLeft.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSetNo--;
				if (imgSetNo < 1)
					imgSetNo = 4;
				LoadMediaData();
			}
			if (ButtonRectRight.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSetNo++;
				if (imgSetNo > 4)
					imgSetNo = 1;
				LoadMediaData();
			}
			break;
		}
		case Event::MouseMoved:
		{
			if (ButtonRectStart.contains(event.mouseMove.x, event.mouseMove.y))
				if (isGameBegin)
					ButtonState_Start = Close_Light;
				else
					ButtonState_Start = Start_Light;
			else
				if (isGameBegin)
					ButtonState_Start = Close_Dark;
				else
					ButtonState_Start = Start_Dark;

			if (ButtonRectHold.contains(event.mouseMove.x, event.mouseMove.y))
				if (isGameHold)
					ButtonState_Hold = Continue_Light;
				else
					ButtonState_Hold = Hold_Light;
			else
				if (isGameHold)
					ButtonState_Hold = Continue_Dark;
				else
					ButtonState_Hold = Hold_Dark;
			break;
		}
		default:
			break;
		}
		player1.Input(&event);
		player2.Input(&event);
	}
}

void Game::gameLogic()
{
	if (isGameHold)
		return;
	float time = clock.getElapsedTime().asSeconds();
	clock.restart();
	player1.timer += time;
	player2.timer += time;
	player1.Logic();
	player2.Logic();
}

void Game::gameDraw()
{
	window.clear();

	sBackground.setPosition(0, 0);
	window.draw(sBackground);
	window.draw(sFrame);
	player1.Draw(&window);
	player2.Draw(&window);

	sCover.setPosition(P1_STAGE_CORNER_X, P1_STAGE_CORNER_Y);
	window.draw(sCover);
	sCover.setPosition(P2_STAGE_CORNER_X, P2_STAGE_CORNER_Y);
	window.draw(sCover);

	DrawButton();
	TextOut();
	DrawResults();
	window.display();
}

void Game::DrawButton()
{
	int ButtonWidth = 110, ButtonHeight = sButtons.getLocalBounds().height;

	sButtons.setTextureRect(IntRect(ButtonState_Start * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(B_START_CORNER_X, B_START_CORNER_Y);
	ButtonRectStart.left = B_START_CORNER_X;
	ButtonRectStart.top = B_START_CORNER_Y;
	ButtonRectStart.width = ButtonWidth;
	ButtonRectStart.height = ButtonHeight;
	window.draw(sButtons);

	sButtons.setTextureRect(IntRect(ButtonState_Hold * ButtonWidth, 0, ButtonWidth, ButtonHeight));
	sButtons.setPosition(B_HOLD_CORNER_X, B_HOLD_CORNER_Y);
	ButtonRectHold.left = B_HOLD_CORNER_X;
	ButtonRectHold.top = B_HOLD_CORNER_Y;
	ButtonRectHold.width = ButtonWidth;
	ButtonRectHold.height = ButtonHeight;
	window.draw(sButtons);

	ButtonWidth = sSwitcher.getLocalBounds().width;
	ButtonHeight = sSwitcher.getLocalBounds().height;

	sSwitcher.setPosition(B_LEFT_CORNER_X, B_LEFT_CORNER_Y);
	ButtonRectLeft.left = B_LEFT_CORNER_X - ButtonWidth / 2;
	ButtonRectLeft.top = B_LEFT_CORNER_Y - ButtonHeight / 2;
	ButtonRectLeft.width = ButtonWidth;
	ButtonRectLeft.height = ButtonHeight;
	window.draw(sSwitcher);

	sSwitcher.setPosition(B_RIGHT_CORNER_X, B_RIGHT_CORNER_Y);
	ButtonRectRight.left = B_RIGHT_CORNER_X - ButtonWidth / 2;
	ButtonRectRight.top = B_RIGHT_CORNER_Y - ButtonHeight / 2;
	ButtonRectRight.width = ButtonWidth;
	ButtonRectRight.height = ButtonHeight;
	sSwitcher.setRotation(180);
	window.draw(sSwitcher);
	sSwitcher.setRotation(0);
}

void Game::gameRun()
{
	do
	{
		gameInitial();
		while (window.isOpen() && gameOver == false)
		{
			gameInput();
			if (isGameBegin)
				gameLogic();
			else
				gameInitial();
			gameDraw();
		}
	} while (!gameQuit);
}

void Game::DrawResults()
{
	int ButtonWidth = 250, ButtonHeight = sGameover.getLocalBounds().height;

	if (player1.gameOver || player2.gameOver)
	{
		sGameover.setTextureRect(IntRect(player1.gameOver * ButtonWidth, 0, ButtonWidth, ButtonHeight));
		sGameover.setPosition(P1_STAGE_CORNER_X + GRIDSIZE * 1.5, 0);
		window.draw(sGameover);

		sGameover.setTextureRect(IntRect(player2.gameOver * ButtonWidth, 0, ButtonWidth, ButtonHeight));
		sGameover.setPosition(P2_STAGE_CORNER_X + GRIDSIZE * 1.5, 0);
		window.draw(sGameover);
	}
}

void Game::TextOut()
{
	int initialX, initialY;
	int CharacterSize = 48;
	stringstream ss;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 0, 0, 255));
	text.setStyle(Text::Bold);

	text.setPosition(P1_NEXT_CORNER_X, P1_NEXT_CORNER_Y);
	ss << player1.score;
	text.setString(ss.str());
	window.draw(text);

	text.setPosition(P2_NEXT_CORNER_X, P2_NEXT_CORNER_Y);
	ss << player2.score;
	text.setString(ss.str());
	window.draw(text);

	//CharacterSize = 18;
	//text.setCharacterSize(CharacterSize);
	//text.setFillColor(Color(255, 255, 255, 255));
	//text.setStyle(Text::Regular||Text::Italic);
	//initialY = P1_NEXT_CORNER_Y + STAGE_HEIGHT * GRIDSIZE;
	//text.setPosition(P1_NEXT_CORNER_X, initialY);
	//text.setString(L"����лFamTrinl�����ṩ��Tetris��Ϸ��������");
	//window.draw(text);
	//initialY += CharacterSize;
	//text.setPosition(P1_NEXT_CORNER_X, initialY);
	//text.setString(L"����л���̵��С��������Tetris˫����	����л���̶�����С����������ϷUI���");
	//window.draw(text);
	//initialY += CharacterSize;
	//text.setPosition(P1_NEXT_CORNER_X, initialY);
	//text.setString(L"����л���ۺ�С���̻���hard drop���ײ����塢bag7��hold��wallkick��nextBlock�ȸ߶��淨");
	//window.draw(text);
	//initialY += CharacterSize;
	//text.setPosition(P1_NEXT_CORNER_X, initialY);
	//text.setString(L"����л����ܰС�����׷��shadowЧ���Լ���������Ч���Ľ���");
	//window.draw(text);
	//initialY += CharacterSize;
	//text.setPosition(P1_NEXT_CORNER_X, initialY);
	//text.setString(L"�����ǵĽ���Ͱ�����������汾��Tetris����Բ����");
	//window.draw(text);

	CharacterSize = 24;
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);
	initialY = INFO_CORNER_Y;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"���1��");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	����WASD");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	�ٽ���Space");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	��������Ctrl");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"���2��");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	����IJKL");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	�ٽ���Enter");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"	��������Ctrl");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"�Դ��ײ����幦��");
	window.draw(text);
	initialY += CharacterSize;
	text.setPosition(INFO_CORNER_X, initialY);
	text.setString(L"�˳���Esc");
	window.draw(text);
}