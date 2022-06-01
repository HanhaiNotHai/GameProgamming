#include <SFML/Graphics.hpp>
#include "Animation.hpp"
#include "AnimatedSprite.hpp"

using namespace sf;

int main(int argc, char* argv[])
{
	RenderWindow window(VideoMode(800, 600), "Example animation");
	//window.setFramerateLimit(1);

	Texture texture;
	texture.loadFromFile("eye.png");
	Animation walkLeft(&texture);
	walkLeft.addFramesLine(4, 2, 0);
	Animation walkRight(&texture);	
	walkRight.addFramesLine(4, 2, 1);

	AnimatedSprite sprite(&walkRight, AnimatedSprite::Playing, seconds(0.1));

	Clock clock;
	while (window.isOpen())
	{
		Time delta = clock.restart();
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		float speed = 50;//游戏对象移动速度
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			sprite.setAnimation(&walkLeft);
			sprite.play();
			sprite.move(-speed * delta.asSeconds(), 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			sprite.setAnimation(&walkRight);
			sprite.play();
			sprite.move(speed * delta.asSeconds(), 0);
		}
		window.clear();
		sprite.update(delta);//根据帧频完成精灵动画的帧更新
		window.draw(sprite);//绘制精灵动画
		window.display();
	}
	return 0;
}