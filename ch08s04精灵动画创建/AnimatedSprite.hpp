#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE__HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>

using namespace sf;

class Animation;
class AnimatedSprite :public Drawable, public Transformable
{
public:
	AnimatedSprite(const AnimatedSprite&) = default;
	AnimatedSprite& operator=(const AnimatedSprite&) = default;
	AnimatedSprite(AnimatedSprite&&) = default;
	AnimatedSprite& operator=(AnimatedSprite&&) = default;

	using FuncType = std::function<void()>;
	static FuncType defaultType;

	enum Status
	{
		Stopped,
		Paused,
		Playing
	};
	AnimatedSprite(Animation* animation = nullptr, Status status = Playing, const Time& deltaTime = seconds(0.15), bool loop = true, int repeat = 0);
	void setAnimation(Animation* animation);
	Animation* getAnimation()const;
	void setFrameTime(Time deltaTime);
	Time getFrameTime()const;

	void setLoop(bool loop);
	bool getLoop()const;
	void setRepeat(int nb);
	int getRepeat()const;

	void play();
	void pause();
	void stop();

	FuncType on_finished;
	Status getStatus()const;
	void setFrame(size_t index);
	void setColor(const Color& color);
	void update(const Time& deltatime);

private:
	Animation* _animation;
	Time _delta;
	Time _elapsed;
	bool _loop;
	int _repeat;
	Status _status;
	size_t _currentFrame;
	Vertex _vertices[4];
	void setFrame(size_t index, bool resetTime);
	virtual void draw(RenderTarget& target, RenderStates states)const override;
};
#endif // !ANIMATEDSPRITE_HPP