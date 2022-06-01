#include "Animation.hpp"

using namespace sf;

//仅仅是一个frame容器
Animation::Animation(Texture* texture) :_texture(texture) {}
Animation::~Animation() {}

void Animation::setTexture(Texture* texture)
{
	_texture = texture;
}
Texture* Animation::getTexture()const
{
	return _texture;
}

Animation& Animation::addFrame(const IntRect& rect)
{
	_frames.emplace_back(rect);
	return *this;
}
//在sprite sheet上，按行加载序列
Animation& Animation::addFramesLine(int number_x, int number_y, int line)
{
	const Vector2u size = _texture->getSize();
	const float delta_x = size.x / float(number_x);
	const float delta_y = size.y / float(number_y);
	for (int i = 0; i < number_x; i++)
		addFrame(IntRect(i * delta_x, line * delta_y, delta_x, delta_y));
	return *this;
}
//在sprite sheet上，按列加载序列
Animation& Animation::addFramesColum(int number_x, int number_y, int colum)
{
	const Vector2u size = _texture->getSize();
	const float delta_x = size.x / float(number_x);
	const float delta_y = size.y / float(number_y);
	for (int i = 0; i < number_y; i++)
		addFrame(IntRect(colum * delta_x, i * delta_y, delta_x, delta_y));
	return *this;
}

size_t Animation::size()const
{
	return _frames.size();
}

const IntRect& Animation::getRect(size_t index)const
{
	return _frames[index];
}