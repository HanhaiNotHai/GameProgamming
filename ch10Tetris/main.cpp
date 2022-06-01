#include "Game.h"

int main()
{
	int a;
	a = 1;
	Game tetris;
	while (tetris.window.isOpen())
	{
		tetris.gameRun();
	}
	return 0;
}