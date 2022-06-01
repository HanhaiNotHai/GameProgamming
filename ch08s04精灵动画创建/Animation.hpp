#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include <vector>
#include <SFML/graphics.hpp>

using namespace sf;

class Animation
{
public:
	Animation(Texture* texture = nullptr);
	~Animation();
	void setTexture(Texture* texture);
	Texture* getTexture()const;
	Animation& addFrame(const IntRect& rect);
	Animation& addFramesLine(int number_x, int number_y, int line);
	Animation& addFramesColum(int number_x, int number_y, int colum);
	size_t size()const;
	const IntRect& getRect(size_t index)const;
private:
	friend class AnimatedSprite;
	std::vector<IntRect> _frames;
	Texture* _texture;
};
#endif // !ANIMATION_HPP