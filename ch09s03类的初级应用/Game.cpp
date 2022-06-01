#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Initial()
{

}

void Game::Input()
{

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
		while (window.isOpen() && !gameOver)
		{
			Input();
			Logic();
			Draw();
		}
	} while (window.isOpen() && !gameQuit);
}