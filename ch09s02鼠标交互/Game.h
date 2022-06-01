#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace sf;

class Game
{
public:
	RenderWindow window;
	Game();
	~Game();
	bool gameOver, gameQuit;
	int Window_width, Window_height, stageWidth, stageHeight;

	Clock mouseClickTimer;

	void Initial();
	void Input();
	void Logic();
	void Draw();
	void Run();
};