#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.h"

const int window_length = 1280;
const int window_width = 720;
const float DT = 1.0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Golf");

	Sphere ball1 = { {640, 360}, {0, 0}, {0, 0}, 50, 1, 255, 0 ,0 };
	Sphere ball2 = { {100, 360}, {1, 0}, {0, 0}, 50, 1, 0, 255 ,0 };
	
	while (window.isOpen())
	{
		sf::Event event;

		window.clear();

		ball1.draw(&window)

		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				break;
			}
		}
	}

	return 0;
}