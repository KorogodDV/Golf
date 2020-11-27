#include <iostream>
#include <SFML/Graphics.hpp>
#include "Objects.h"

const int window_length = 1280;
const int window_width = 720;
const float DT = 1.0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Golf");

	Sphere ball1 = { {900, 360}, {0, 0}, {0, 0}, 50, 1, 255, 0 ,0 };
	Sphere ball2 = { {400, 500}, {-0.2, -0.2}, {0, 0}, 20, 1, 0, 255 ,0 };

	sf::ConvexShape rect1;
	rect1.setPointCount(4);
	rect1.setPoint(0, { 200, 300 });
	rect1.setPoint(1, { 500, 300 });
	rect1.setPoint(2, { 500, 400 });
	rect1.setPoint(3, { 200, 400 });
	rect1.setFillColor(sf::Color(255, 0, 0));
	
	sf::ConvexShape rect2;
	rect2.setPointCount(4);
	rect2.setPoint(0, { 200, 200 });
	rect2.setPoint(1, { 300, 200 });
	rect2.setPoint(2, { 300, 400 });
	rect2.setPoint(3, { 200, 400 });
	rect2.setFillColor(sf::Color(255, 0, 0));

	sf::ConvexShape walls[10];
	
	while (window.isOpen())
	{
		sf::Event event;

		window.clear();

		window.draw(rect1);
		window.draw(rect2);
		ball1.draw(&window);
		ball2.draw(&window);

		window.display();

		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				break;
			}

			//if ((event.type == sf::Event::KeyReleased) /*|| (event.key.code == sf::Keyboard::Enter)*/)
			//{
			//	std::cout << "Enter";
			//}
		}

		if (ball1.checkCollisionTwoSpheres(&ball2))
			ball1.collideSpheres(&ball2, &window);

		ball2.collide(&rect1, DT);
		ball2.collide(&rect2, DT);

		ball1.move(DT);
		ball2.move(DT);

	}

	return 0;
}