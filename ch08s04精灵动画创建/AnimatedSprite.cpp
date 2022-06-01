#include "AnimatedSprite.hpp"
#include "Animation.hpp"

#include <cassert>

using namespace sf;

//动画播放类
AnimatedSprite::FuncType AnimatedSprite::defaultType = []()->void {};

//构造函数通过传参对内部变量进行赋值
AnimatedSprite::AnimatedSprite(Animation* animation, Status status, const Time& deltaTime, bool loop, int repeat) :on_finished(defaultType), _delta(deltaTime), _loop(loop), _repeat(repeat), _status(status)
{
	setAnimation(animation);
}

//这个函数只在新旧纹理不同的情况才会更新纹理，并将帧重设为动画的第一帧
//注意，新动画中必须至少存在一个帧图像
void AnimatedSprite::setAnimation(Animation* animation)
{
	if (_animation != animation)
	{
		_animation = animation;
		_elapsed = Time::Zero;
		_currentFrame = 0;
		setFrame(0, true);
	}
}
Animation* AnimatedSprite::getAnimation()const
{
	return _animation;
}

void AnimatedSprite::setFrameTime(Time deltaTime)
{
	_delta = deltaTime;
}
Time AnimatedSprite::getFrameTime()const
{
	return _delta;
}

void AnimatedSprite::setLoop(bool loop)
{
	_loop = loop;
}
bool AnimatedSprite::getLoop()const
{
	return _loop;
}

void AnimatedSprite::setRepeat(int nb)
{
	_repeat = nb;
}
int AnimatedSprite::getRepeat()const
{
	return _repeat;
}

void AnimatedSprite::play()
{
	_status = Playing;
}
void AnimatedSprite::pause()
{
	_status = Paused;
}
void AnimatedSprite::stop()
{
	_status = Stopped;
	_currentFrame = 0;
	setFrame(0, true);
}
AnimatedSprite::Status AnimatedSprite::getStatus()const
{
	return _status;
}

//此函数将当前帧更改为从_animation动画序列中获取的新帧
void AnimatedSprite::setFrame(size_t index)
{
	assert(_animation);
	_currentFrame = index % _animation->size();
	setFrame(_currentFrame, true);
}

//此函数用于改变所显示纹理图像的颜色掩码
//为此，将纹理的4个内部顶点的颜色参数设置为新颜色值
void AnimatedSprite::setColor(const Color& color)
{
	for (int i = 0; i < 4; i++)
		_vertices[i].color = color;
}

void AnimatedSprite::update(const Time& deltaTime)
{
	if (_status == Playing && _animation)
	{
		_elapsed += deltaTime;
		//帧切换
		if (_elapsed > _delta)
		{
			_elapsed -= _delta;
			if (_currentFrame + 1 < _animation->size())
				++_currentFrame;
			//已经是最后一帧
			else
			{
				_currentFrame = 0;
				//是否开启下次循环
				if (!_loop)
				{
					--_repeat;
					//动画播放结束
					if (_repeat <= 0)
					{
						_status = Stopped;
						on_finished();
					}
				}
			}
		}
		//更新当前帧
		setFrame(_currentFrame, false);
	}
}

//该函数的目的是将内部animation类对象_animation第index帧的属性（位置和坐标纹理）赋值给当前动画帧的4个顶点
void AnimatedSprite::setFrame(size_t index, bool resetTime)
{
	if (_animation)
	{
		IntRect rect = _animation->getRect(index);

		_vertices[0].position = Vector2f(0.f, 0.f);
		_vertices[1].position = Vector2f(0.f, static_cast<float>(rect.height));
		_vertices[2].position = Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
		_vertices[3].position = Vector2f(static_cast<float>(rect.width), 0.f);

		float left = static_cast<float>(rect.left);
		float right = left + static_cast<float>(rect.width);
		float top = static_cast<float>(rect.top);
		float bottom = top + static_cast<float>(rect.height);

		_vertices[0].texCoords = Vector2f(left, top);
		_vertices[1].texCoords = Vector2f(left, bottom);
		_vertices[2].texCoords = Vector2f(right, bottom);
		_vertices[3].texCoords = Vector2f(right, top);
	}

	if (resetTime)
		_elapsed = Time::Zero;
}

void AnimatedSprite::draw(RenderTarget& target, RenderStates states)const
{
	if (_animation && _animation->_texture)
	{
		states.transform *= getTransform();
		states.texture = _animation->_texture;
		target.draw(_vertices, 4, Quads, states);
	}
}