#include "AnimatedSprite.hpp"
#include "Animation.hpp"

#include <cassert>

using namespace sf;

//����������
AnimatedSprite::FuncType AnimatedSprite::defaultType = []()->void {};

//���캯��ͨ�����ζ��ڲ��������и�ֵ
AnimatedSprite::AnimatedSprite(Animation* animation, Status status, const Time& deltaTime, bool loop, int repeat) :on_finished(defaultType), _delta(deltaTime), _loop(loop), _repeat(repeat), _status(status)
{
	setAnimation(animation);
}

//�������ֻ���¾�����ͬ������Ż������������֡����Ϊ�����ĵ�һ֡
//ע�⣬�¶����б������ٴ���һ��֡ͼ��
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

//�˺�������ǰ֡����Ϊ��_animation���������л�ȡ����֡
void AnimatedSprite::setFrame(size_t index)
{
	assert(_animation);
	_currentFrame = index % _animation->size();
	setFrame(_currentFrame, true);
}

//�˺������ڸı�����ʾ����ͼ�����ɫ����
//Ϊ�ˣ��������4���ڲ��������ɫ��������Ϊ����ɫֵ
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
		//֡�л�
		if (_elapsed > _delta)
		{
			_elapsed -= _delta;
			if (_currentFrame + 1 < _animation->size())
				++_currentFrame;
			//�Ѿ������һ֡
			else
			{
				_currentFrame = 0;
				//�Ƿ����´�ѭ��
				if (!_loop)
				{
					--_repeat;
					//�������Ž���
					if (_repeat <= 0)
					{
						_status = Stopped;
						on_finished();
					}
				}
			}
		}
		//���µ�ǰ֡
		setFrame(_currentFrame, false);
	}
}

//�ú�����Ŀ���ǽ��ڲ�animation�����_animation��index֡�����ԣ�λ�ú�����������ֵ����ǰ����֡��4������
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