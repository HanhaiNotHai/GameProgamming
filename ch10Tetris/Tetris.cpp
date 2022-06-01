#include "Tetris.h"

Tetris::Tetris()
{
	dx = 0;
	rotate = false;
	colorNum = 1;
	timer = 0;
}

Tetris::~Tetris() {}

int Tetris::holdcolorNum = 0, Tetris::holdShapeNum = 0;
Vector2i Tetris::holdSquare[4] = { {0,0},{0,0},{0,0},{0,0} };

void Tetris::Initial(Texture* tex)
{
	memset(Field, 0, sizeof(Field));
	tTiles = tex;
	dx = 0;
	rotate = false;
	hold = false;

	colorNum = 1;
	timer = 0;
	delay = DELAYVALUE;
	holdSquareCornPoint = { HOLD_CORNER_X,HOLD_CORNER_Y };
	b7Int = 0;
	if (role == rolePLAYER1)
	{
		mCornPoint = { P1_STAGE_CORNER_X,P1_STAGE_CORNER_Y };
		nextSquareCornPoint = { P1_NEXT_CORNER_X,P1_NEXT_CORNER_Y };
	}
	if (role == rolePLAYER2)
	{
		mCornPoint = { P2_STAGE_CORNER_X,P2_STAGE_CORNER_Y };
		nextSquareCornPoint = { P2_NEXT_CORNER_X,P2_NEXT_CORNER_Y };
	}
	sTiles.setTexture(*tTiles);

	//初始化方块
	colorNum = 1 + rand() % 7;
	currentShapeNum = rand() % 7;
	nextcolorNum = 1 + rand() % 7;
	nextShapeNum = rand() % 7;
	for (int i = 0; i < 4; i++)
	{
		currentSquare[i].x = Figures[currentShapeNum][i] % 2 + STAGE_WIDTH / 2;
		currentSquare[i].y = Figures[currentShapeNum][i] / 2;
		nextSquare[i].x = Figures[nextShapeNum][i] % 2;
		nextSquare[i].y = Figures[nextShapeNum][i] / 2;
		animationRow[i] = -1;
	}

	holdShapeNum = -1;

	animationFlag = true;
	animationCtrlValue = 1.0;
	score = 0;
}

void Tetris::Input(Event* event)
{
	switch (role)
	{
	case rolePLAYER1:
	{
		switch (event->type)
		{
		case Event::KeyPressed:
		{
			switch (event->key.code)
			{
			case Keyboard::W:
			{
				if (currentShapeNum != shapeO)
					rotate = true;
				break;
			}
			case Keyboard::A:
			{
				dx = -1;
				break;
			}
			case Keyboard::D:
			{
				dx = 1;
				break;
			}
			case Keyboard::S:
			{
				delay = DELAYVALUE / 6;
				break;
			}
			default:
				break;
			}
			break;
		}
		case Event::KeyReleased:
		{
			switch (event->key.code)
			{
			case Keyboard::A:
			case Keyboard::D:
			{
				dx = 0;
				break;
			}
			case Keyboard::S:
			{
				delay = DELAYVALUE;
				break;
			}
			case Keyboard::LControl:
			{
				hold = true;
				break;
			}
			case Keyboard::Space:
			{
				hardDrop = true;
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case rolePLAYER2:
	{
		switch (event->type)
		{
		case Event::KeyPressed:
		{
			switch (event->key.code)
			{
			case Keyboard::I:
			{
				if (currentShapeNum != shapeO)
					rotate = true;
				break;
			}
			case Keyboard::J:
			{
				dx = -1;
				break;
			}
			case Keyboard::L:
			{
				dx = 1;
				break;
			}
			case Keyboard::K:
			{
				delay = DELAYVALUE / 6;
				break;
			}
			default:
				break;
			}
			break;
		}
		case Event::KeyReleased:
		{
			switch (event->key.code)
			{
			case Keyboard::J:
			case Keyboard::L:
			{
				dx = 0;
				break;
			}
			case Keyboard::K:
			{
				delay = DELAYVALUE;
				break;
			}
			case Keyboard::RControl:
			{
				hold = true;
				break;
			}
			case Keyboard::Enter:
			{
				hardDrop = true;
				break;
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void Tetris::Logic()
{
	if (!animationFlag)
		traditionLogic();

	if (newShapeFlag)
	{
		if (!animationFlag)
		{
			checkLine();
			if (!animationFlag)
				newShapeFunc();
			isWin();
		}
		else
		{
			animationCtrlValue -= 0.1;
			if (animationCtrlValue < 0)
			{
				animationFlag = false;
				animationCtrlValue = 1.0;
				memset(animationRow, -1, sizeof(animationRow));
				clearLine();
				newShapeFunc();
			}
		}
	}
}

void Tetris::holdFunc()
{
	Vector2i backUpSquare[4];
	tempcolorNum = holdcolorNum;
	tempShapeNum = holdShapeNum;
	holdcolorNum = colorNum;
	holdShapeNum = currentShapeNum;

	for (int i = 0; i < 4; i++)
	{
		holdSquare[i].x = Figures[holdShapeNum][i] % 2;
		holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		tempSquare[i].x = Figures[tempShapeNum][i] % 2;
		tempSquare[i].y = Figures[tempShapeNum][i] / 2;
		backUpSquare[i] = currentSquare[i];
	}

	if (tempShapeNum < 0)
		newShapeFunc();
	else
	{
		colorNum = tempcolorNum;
		currentShapeNum = tempShapeNum;

		int minCurrentX = currentSquare[0].x, minCurrentY = currentSquare[0].y,
			minTempX = tempSquare[0].x, minTempY = tempSquare[0].y,
			dx, dy;

		for (int i = 1; i < 4; i++)
		{
			if (currentSquare[i].x < minCurrentX)
				minCurrentX = currentSquare[i].x;
			if (currentSquare[i].y < minCurrentY)
				minCurrentY = currentSquare[i].y;
			if (tempSquare[i].x < minTempX)
				minTempX = tempSquare[i].x;
			if (tempSquare[i].y < minTempY)
				minTempY = tempSquare[i].y;
		}
		dx = minCurrentX - minTempX;
		dy = minCurrentY - minTempY;
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i].x = tempSquare[i].x + dx;
			currentSquare[i].y = tempSquare[i].y + dy;
			holdSquare[i].x = Figures[holdShapeNum][i] % 2;
			holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		}
	}
	if (!hitTest(currentSquare))
	{
		colorNum = holdcolorNum;
		holdcolorNum = tempcolorNum;
		holdShapeNum = tempShapeNum;
		for (int i = 0; i < 4; i++)
		{
			currentSquare[i] = backUpSquare[i];
			holdSquare[i].x = Figures[holdShapeNum][i] % 2;
			holdSquare[i].y = Figures[holdShapeNum][i] / 2;
		}
	}
}

void Tetris::xmove()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].x += dx;
	}
	if (!hitTest(currentSquare))
		for (int i = 0; i < 4; i++)
			currentSquare[i] = tempSquare[i];
}

void Tetris::rotateFunc()
{
	int originalHeight = currentSquare[0].y;
	for (int j = 0; j < 4; j++)
	{
		Vector2i p = currentSquare[j];
		for (int i = 0; i < 4; i++)
		{
			int x = currentSquare[i].y - p.y;
			int y = currentSquare[i].x - p.x;
			currentSquare[i].x = p.x - x;
			currentSquare[i].y = p.y + y;
		}

		if (hitTest(currentSquare))
		{
			int deltaY = 0;
			deltaY = currentSquare[0].y - originalHeight;
			if (deltaY != 0)
				for (int i = 0; i < 4; i++)
					currentSquare[i].y -= deltaY;
			if (!hitTest(currentSquare))
				for (int i = 0; i < 4; i++)
					currentSquare[i] = tempSquare[i];
			else
				break;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				currentSquare[i] = tempSquare[i];
		}
	}
}

void Tetris::ymove()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;
	}

	if (!hitTest(currentSquare))
	{
		for (int i = 0; i < 4; i++)
			Field[tempSquare[i].y][tempSquare[i].x] = colorNum;

		//colorNum = nextcolorNum;
		//currentShapeNum = nextShapeNum;

		//nextcolorNum = 1 + rand() % 7;
		////nextShapeNum = rand() % 7;
		//nextShapeNum = Bag7();

		//for (int i = 0; i < 4; i++)
		//{
		//	currentSquare[i] = nextSquare[i];
		//	currentSquare[i].x += STAGE_WIDTH / 2;
		//	nextSquare[i].x = Figures[nextShapeNum][i] % 2;
		//	nextSquare[i].y = Figures[nextShapeNum][i] / 2;
		//}

		newShapeFlag = true;
	}
}

void Tetris::checkLine()
{
	int k = STAGE_HEIGHT - 1;
	int yCount = 0;
	for (int i = STAGE_HEIGHT - 1; i > 0; i--)
	{
		int xCount = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j])
				xCount++;
		}
		if (xCount < STAGE_WIDTH)
			k--;
		else
		{
			animationRow[yCount] = i;
			yCount++;
			animationFlag = true;
		}
	}
	switch (yCount)
	{
	case 1:
	{
		score += 10;
		break;
	}
	case 2:
	{
		score += 30;
		break;
	}
	case 3:
	{
		score += 60;
		break;
	}
	case 4:
	{
		score += 100;
		break;
	}
	default:
		break;
	}
}

void Tetris::slowLoading()
{
	for (int i = 0; i < 4; i++)
	{
		tempSquare[i] = currentSquare[i];
		currentSquare[i].y += 1;
	}
	if (!hitTest(currentSquare))
		delay = DELAYVALUE * 3;
	else if (delay > DELAYVALUE)
		delay = DELAYVALUE;
	for (int i = 0; i < 4; i++)
		currentSquare[i] = tempSquare[i];
}

void Tetris::hardDropFunc()
{
	for (int i = 0; i < 4; i++)
		currentSquare[i] = shadowSquare[i];
}

void Tetris::shadow()
{
	for (int i = 0; i < 4; i++)
		shadowSquare[i] = currentSquare[i];
	for (int j = 0; j < STAGE_HEIGHT; j++)
	{
		for (int i = 0; i < 4; i++)
			shadowSquare[i].y += 1;
		if (!hitTest(shadowSquare))
		{
			for (int i = 0; i < 4; i++)
				shadowSquare[i].y -= 1;
			break;
		}
	}
}

void Tetris::Draw(RenderWindow* w)
{
	window = w;
	if (!animationFlag)
	{
		//shadow
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(shadowSquare[i].x * GRIDSIZE, shadowSquare[i].y * GRIDSIZE);
			sTiles.setColor(Color(77, 77, 77, 127));
			sTiles.move(mCornPoint.x, mCornPoint.y);
			window->draw(sTiles);
			sTiles.setColor(Color(255, 255, 255, 255));
		}

		//current
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(colorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(currentSquare[i].x * GRIDSIZE, currentSquare[i].y * GRIDSIZE);
			sTiles.move(mCornPoint.x, mCornPoint.y);
			window->draw(sTiles);
		}
	}

	//field
	for (int i = 0; i < STAGE_HEIGHT; i++)
		if (i == animationRow[0] || i == animationRow[1] || i == animationRow[2] || i == animationRow[3])
			animationFunc(i);
		else
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				if (Field[i][j] == 0)
					continue;
				sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
				sTiles.move(mCornPoint.x, mCornPoint.y);
				window->draw(sTiles);
			}

	//next
	for (int i = 0; i < 4; i++)
	{
		sTiles.setTextureRect(IntRect(nextcolorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
		sTiles.setPosition(nextSquare[i].x * GRIDSIZE, nextSquare[i].y * GRIDSIZE);
		sTiles.move(nextSquareCornPoint.x, nextSquareCornPoint.y);
		window->draw(sTiles);
	}

	//hold
	if (holdShapeNum > -1)
		for (int i = 0; i < 4; i++)
		{
			sTiles.setTextureRect(IntRect(holdcolorNum * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
			sTiles.setPosition(holdSquare[i].x * GRIDSIZE, holdSquare[i].y * GRIDSIZE);
			sTiles.move(holdSquareCornPoint.x, holdSquareCornPoint.y);
			window->draw(sTiles);
		}
}

bool Tetris::hitTest()
{
	for (int i = 0; i < 4; i++)
		if (currentSquare[i].x < 0 || currentSquare[i].x >= STAGE_WIDTH || currentSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (Field[currentSquare[i].y][currentSquare[i].x])
			return false;
	return true;
}

bool Tetris::hitTest(Vector2i hitSquare[])
{
	for (int i = 0; i < 4; i++)
		if (hitSquare[i].x < 0 || hitSquare[i].x >= STAGE_WIDTH || hitSquare[i].y >= STAGE_HEIGHT)
			return false;
		else if (Field[hitSquare[i].y][hitSquare[i].x])
			return false;
	return true;
}

int Tetris::Bag7()
{
	int num;
	//srand(time(NULL));
	num = rand() % 7;
	for (int i = 0; i < b7Int; i++)
		if (b7array[i] == num)
		{
			i = -1;
			num = rand() % 7;
		}
	b7array[b7Int] = num;

	b7Int++;
	if (b7Int == 7)
	{
		b7Int = 0;
		for (int i = 0; i < 7; i++)
			b7array[i] = 0;
	}
	return num;
}

int Tetris::sBag7()
{
	b7Int = (b7Int + 1) % 7;
	if (b7Int == 0)
	{
		bool b[7] = { };
		int num;

		for (int i = 0; i < 7; i++)
		{
			num = rand() % 7;
			while (b[num])
				num = rand() % 7;
			b7array[i] = num;
			b[num] = true;
		}
	}
	else
	{
		return b7array[b7Int];
	}
}

void Tetris::traditionLogic()
{
	//hold
	if (hold)
	{
		holdFunc();
		hold = false;
	}

	//<-水平move->
	xmove();

	//rotate
	if (rotate)
	{
		rotateFunc();
		rotate = false;
	}

	//<-底部缓冲->//
	slowLoading();

	//Tick下落
	if (timer > delay)
	{
		ymove();
		timer = 0;
	}

	//投影
	shadow();
	if (hardDrop)
	{
		hardDropFunc();
		hardDrop = false;
	}
}

void Tetris::clearLine()
{
	int k = STAGE_HEIGHT - 1;
	for (int i = STAGE_HEIGHT - 1; i > 0; i--)
	{
		int xCount = 0;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (Field[i][j])
				xCount++;
			Field[k][j] = Field[i][j];
		}
		if (xCount < STAGE_WIDTH)
			k--;
	}
}

void Tetris::newShapeFunc()
{
	colorNum = nextcolorNum;
	currentShapeNum = nextShapeNum;

	nextcolorNum = 1 + rand() % 7;
	nextShapeNum = Bag7();

	for (int i = 0; i < 4; i++)
	{
		currentSquare[i] = nextSquare[i];
		currentSquare[i].x += STAGE_WIDTH / 2;
		nextSquare[i].x = Figures[nextShapeNum][i] % 2;
		nextSquare[i].y = Figures[nextShapeNum][i] / 2;
	}

	shadow();
	newShapeFlag = false;
}

void Tetris::animationFunc(int i)
{
	Vector2f p;
	sTiles.scale(animationCtrlValue, animationCtrlValue);
	p = sTiles.getOrigin();
	sTiles.setOrigin(GRIDSIZE / 2, GRIDSIZE / 2);
	sTiles.rotate(360 * animationCtrlValue);
	for (int j = 0; j < STAGE_WIDTH; j++)
	{
		sTiles.setTextureRect(IntRect(Field[i][j] * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
		sTiles.setPosition(j * GRIDSIZE, i * GRIDSIZE);
		sTiles.move(mCornPoint.x + GRIDSIZE / 2, mCornPoint.y + GRIDSIZE / 2);
		window->draw(sTiles);
	}
	sTiles.setScale(1.0, 1.0);
	sTiles.setRotation(0);
	sTiles.setOrigin(p);
}

void Tetris::isWin()
{
	if (Field[2][5] || Field[2][6])
		gameOver = true;
}