#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
	RenderWindow window(VideoMode(200, 200), "SFML works!");
	window.setKeyRepeatEnabled(false);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::A)
					cout << "A";
			if (event.type == Event::KeyReleased)
				if (event.key.code == Keyboard::A)
					cout << "A¡ü";
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::J)
					cout << "J";
		}

		window.clear();
		window.display();
	}

	return 0;
}