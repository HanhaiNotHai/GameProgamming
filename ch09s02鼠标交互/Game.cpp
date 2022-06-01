#include "Game.h"

Game::Game()
{
	gameOver = false;
	gameQuit = false;

	Window_width = 860;
	Window_height = 600;

	window.create(VideoMode(Window_width, Window_height), L"MineSweeper by ÷Ï∫∆—Û");
}

Game::~Game()
{

}

void Game::Initial()
{
	window.setFramerateLimit(60);
}

void Game::Input()
{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			gameQuit = true;
		}
		if (event.type == Event::EventType::KeyReleased)
		{
			if (event.key.code == Keyboard::Escape)
			{
				cout << "Escape" << endl << endl;
				window.close();
				gameQuit = true;
			}
			else
			{
				cout << event.key.code << endl << endl;
			}
		}
		if (event.key.alt && event.key.code == Keyboard::Z)
		{
			cout << "alt + z" << endl << endl;
		}
		if (event.type == Event::MouseButtonPressed)
		{
			if (mouseClickTimer.getElapsedTime().asMilliseconds() > 500)
			{
				if (event.mouseButton.button == Mouse::Left)
					cout << "Mouse::Left Pressed" << endl;
				else if (event.mouseButton.button == Mouse::Right)
					cout << "Mouse::Right Pressed" << endl;
			}
			else
				cout << "Mouse Double Clicked" << endl;
			cout << "mouse x:" << event.mouseButton.x << endl
				<< "mouse y:" << event.mouseButton.y << endl << endl;
		}
		if (event.type == Event::MouseButtonReleased)
		{
			mouseClickTimer.restart();
			if (event.mouseButton.button == Mouse::Left)
				cout << "Mouse::Left Released" << endl;
			else if (event.mouseButton.button == Mouse::Right)
				cout << "Mouse::Right Released" << endl;
			cout << "mouse x:" << event.mouseButton.x << endl
				<< "mouse y:" << event.mouseButton.y << endl << endl;
		}
		if (event.type == Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
				cout << "wheel type: vertical" << endl;
			else if (event.mouseWheelScroll.wheel == Mouse::HorizontalWheel)
				cout << "wheel type: horizontal" << endl;
			cout << "wheel movement" << event.mouseWheelScroll.delta << endl << endl;
		}
		if (event.type == Event::MouseEntered)
		{
			cout << "mouse entered" << endl << endl;
		}
		if (event.type == Event::MouseLeft)
		{
			cout << "mouse left" << endl << endl;
		}
		//if (event.type == Event::MouseMoved)
		//{
		//	cout << "new mouse x: " << event.mouseMove.x << endl
		//		<< "new mouse y: " << event.mouseMove.y << endl << endl;
		//}
	}
	//Mouse::setPosition(Vector2i(100, 100), window);
}

void Game::Logic()
{

}

void Game::Draw()
{

}

void Game::Run()
{
	do
	{
		Initial();
		while (window.isOpen() && gameOver == false)
		{
			Input();
			Logic();
			Draw();
		}
	} while (!gameQuit);
}