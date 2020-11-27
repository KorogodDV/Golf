#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
	sf::ConvexShape walls[11];
	for (int i = 0; i < 11; i++)
	{
		walls[i].setPointCount(4);
		walls[i].setFillColor(sf::Color(128, 128, 128));
	}

	walls[1].setPoint(0, sf::Vector2f(0, 0));
	walls[1].setPoint(1, sf::Vector2f(1280, 0));
	walls[1].setPoint(2, sf::Vector2f(1280, 40));
	walls[1].setPoint(3, sf::Vector2f(0, 40));

	walls[3].setPoint(0, sf::Vector2f(0, 680));
	walls[3].setPoint(1, sf::Vector2f(1280, 680));
	walls[3].setPoint(2, sf::Vector2f(1280, 720));
	walls[3].setPoint(3, sf::Vector2f(0, 720));

	walls[0].setPoint(0, sf::Vector2f(0, 0));
	walls[0].setPoint(1, sf::Vector2f(240, 0));
	walls[0].setPoint(2, sf::Vector2f(240, 720));
	walls[0].setPoint(3, sf::Vector2f(0, 720));

	walls[10].setPoint(0, sf::Vector2f(1040, 0));
	walls[10].setPoint(1, sf::Vector2f(1280, 0));
	walls[10].setPoint(2, sf::Vector2f(1280, 720));
	walls[10].setPoint(3, sf::Vector2f(1040, 720));

	walls[2].setPoint(0, sf::Vector2f(400, 0));
	walls[2].setPoint(1, sf::Vector2f(440, 0));
	walls[2].setPoint(2, sf::Vector2f(440, 560));
	walls[2].setPoint(3, sf::Vector2f(400, 560));

	walls[4].setPoint(0, sf::Vector2f(560, 160));
	walls[4].setPoint(1, sf::Vector2f(600, 160));
	walls[4].setPoint(2, sf::Vector2f(600, 720));
	walls[4].setPoint(3, sf::Vector2f(560, 720));

	walls[6].setPoint(0, sf::Vector2f(840, 0));
	walls[6].setPoint(1, sf::Vector2f(880, 0));
	walls[6].setPoint(2, sf::Vector2f(880, 560));
	walls[6].setPoint(3, sf::Vector2f(840, 560));

	walls[5].setPoint(0, sf::Vector2f(560, 160));
	walls[5].setPoint(1, sf::Vector2f(760, 160));
	walls[5].setPoint(2, sf::Vector2f(760, 200));
	walls[5].setPoint(3, sf::Vector2f(560, 200));

	walls[8].setPoint(0, sf::Vector2f(560, 400));
	walls[8].setPoint(1, sf::Vector2f(760, 400));
	walls[8].setPoint(2, sf::Vector2f(760, 440));
	walls[8].setPoint(3, sf::Vector2f(560, 440));

	walls[7].setPoint(0, sf::Vector2f(680, 280));
	walls[7].setPoint(1, sf::Vector2f(880, 280));
	walls[7].setPoint(2, sf::Vector2f(880, 320));
	walls[7].setPoint(3, sf::Vector2f(680, 320));

	walls[9].setPoint(0, sf::Vector2f(680, 520));
	walls[9].setPoint(1, sf::Vector2f(880, 520));
	walls[9].setPoint(2, sf::Vector2f(880, 560));
	walls[9].setPoint(3, sf::Vector2f(680, 560));
	
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Window");
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		
		window.clear();
		for (int i = 0; i < 11; i++)
		{
			window.draw(walls[i]);
		}
		window.display();
	}

	return 0;
}
