#include <SFML/Graphics.hpp>
#include "Sphere.h"
#include "Floor.h"
#include "Player.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>

void defWall(sf::ConvexShape* wall, sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
{
	wall->setPoint(0, A);
	wall->setPoint(1, B);
	wall->setPoint(2, C);
	wall->setPoint(3, D);
};

bool isBallAtRest(Sphere ball)
{
	return len(ball.speed) == 0;
}

bool isCursorOnController(sf::Vector2f curpos, Player player)
{
	return len(curpos - player.pos) < player.radius;
}

const int window_length = 1280;
const int window_width = 720;
const float DT = 1.0;

void endPlay(sf::RenderWindow* window, Player player, sf::Font* font)
{
	window->clear(sf::Color(player.red, player.green, player.blue));

	sf::Text wincongrat(player.name + std::string(" PLAYER HAS WON IN ") + std::to_string(player.count) + std::string(" MOVES"), *font, 50);
	wincongrat.setFillColor(sf::Color(255, 255, 255));
	wincongrat.setPosition(210, 335);

	window->draw(wincongrat);

	wincongrat.setString(std::string("(press any button to close this window)"));
	wincongrat.setCharacterSize(25);
	wincongrat.setPosition(410, 385);
	window->draw(wincongrat);

	window->display();
};

int main()
{
	// CREATING WINDOW
	sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Golf");
	window.setFramerateLimit(60);

	// FONT UPLOADING
	sf::Font font;
	font.loadFromFile("arial.ttf");

	//INTIALIZING WALLS

	sf::ConvexShape walls[11];
	for (int i = 0; i < 11; i++)
	{
		walls[i].setPointCount(4);
		walls[i].setFillColor(sf::Color(128, 128, 128));
	}

	defWall(&walls[0], sf::Vector2f(0, 0), sf::Vector2f(240, 0), sf::Vector2f(240, 720), sf::Vector2f(0, 720));
	defWall(&walls[1], sf::Vector2f(0, 0), sf::Vector2f(1280, 0), sf::Vector2f(1280, 40), sf::Vector2f(0, 40));
	defWall(&walls[2], sf::Vector2f(400, 0), sf::Vector2f(440, 0), sf::Vector2f(440, 560), sf::Vector2f(400, 560));
	defWall(&walls[3], sf::Vector2f(0, 680), sf::Vector2f(1280, 680), sf::Vector2f(1280, 720), sf::Vector2f(0, 720));
	defWall(&walls[4], sf::Vector2f(560, 160), sf::Vector2f(600, 160), sf::Vector2f(600, 720), sf::Vector2f(560, 720));
	defWall(&walls[5], sf::Vector2f(560, 160), sf::Vector2f(760, 160), sf::Vector2f(760, 200), sf::Vector2f(560, 200));
	defWall(&walls[6], sf::Vector2f(840, 0), sf::Vector2f(880, 0), sf::Vector2f(880, 560), sf::Vector2f(840, 560));
	defWall(&walls[7], sf::Vector2f(680, 280), sf::Vector2f(880, 280), sf::Vector2f(880, 320), sf::Vector2f(680, 320));
	defWall(&walls[8], sf::Vector2f(560, 400), sf::Vector2f(760, 400), sf::Vector2f(760, 440), sf::Vector2f(560, 440));
	defWall(&walls[9], sf::Vector2f(680, 520), sf::Vector2f(880, 520), sf::Vector2f(880, 560), sf::Vector2f(680, 560));
	defWall(&walls[10], sf::Vector2f(1040, 0), sf::Vector2f(1280, 0), sf::Vector2f(1280, 720), sf::Vector2f(1040, 720));
	

	//INITIALIZING SAND AND BUSTERS

	const int N = 58;
	Floor floor[N];

	floor[0] = { -1, sf::Vector2f(0, 0), sf::Vector2f(320, 480), sf::Vector2f(400, 480), sf::Vector2f(400, 560), sf::Vector2f(320, 560) };
	floor[1] = { 1, sf::Vector2f(0, -1), sf::Vector2f(440, 480), sf::Vector2f(480, 480), sf::Vector2f(480, 520), sf::Vector2f(440, 520) };
	floor[2] = { 1, sf::Vector2f(0, -1), sf::Vector2f(480, 480), sf::Vector2f(520, 480), sf::Vector2f(520, 520), sf::Vector2f(480, 520) };
	floor[3] = { 1, sf::Vector2f(0, -1), sf::Vector2f(520, 480), sf::Vector2f(560, 480), sf::Vector2f(560, 520), sf::Vector2f(520, 520) };
	floor[4] = { 1, sf::Vector2f(0, -1), sf::Vector2f(440, 520), sf::Vector2f(480, 520), sf::Vector2f(480, 560), sf::Vector2f(440, 560) };
	floor[5] = { 1, sf::Vector2f(0, -1), sf::Vector2f(480, 520), sf::Vector2f(520, 520), sf::Vector2f(520, 560), sf::Vector2f(480, 560) };
	floor[6] = { 1, sf::Vector2f(0, -1), sf::Vector2f(520, 520), sf::Vector2f(560, 520), sf::Vector2f(560, 560), sf::Vector2f(520, 560) };
	floor[7] = { 1, sf::Vector2f(0, 1), sf::Vector2f(600, 280), sf::Vector2f(640, 280), sf::Vector2f(640, 320), sf::Vector2f(600, 320) };
	floor[8] = { 1, sf::Vector2f(0, 1), sf::Vector2f(640, 280), sf::Vector2f(680, 280), sf::Vector2f(680, 320), sf::Vector2f(640, 320) };
	floor[9] = { 1, sf::Vector2f(1, 0), sf::Vector2f(600, 320), sf::Vector2f(640, 320), sf::Vector2f(640, 360), sf::Vector2f(600, 360) };
	floor[10] = { 1, sf::Vector2f(1, 0), sf::Vector2f(600, 360), sf::Vector2f(640, 360), sf::Vector2f(640, 400), sf::Vector2f(600, 400) };
	floor[11] = { 1, sf::Vector2f(1, 0), sf::Vector2f(640, 320), sf::Vector2f(680, 320), sf::Vector2f(680, 360), sf::Vector2f(640, 360) };
	floor[12] = { 1, sf::Vector2f(1, 0), sf::Vector2f(640, 360), sf::Vector2f(680, 360), sf::Vector2f(680, 400), sf::Vector2f(640, 400) };
	floor[13] = { 1, sf::Vector2f(1, 0), sf::Vector2f(680, 320), sf::Vector2f(720, 320), sf::Vector2f(720, 360), sf::Vector2f(680, 360) };
	floor[14] = { 1, sf::Vector2f(1, 0), sf::Vector2f(680, 360), sf::Vector2f(720, 360), sf::Vector2f(720, 400), sf::Vector2f(680, 400) };
	floor[15] = { 1, sf::Vector2f(1, 0), sf::Vector2f(720, 320), sf::Vector2f(760, 320), sf::Vector2f(760, 360), sf::Vector2f(720, 360) };
	floor[16] = { 1, sf::Vector2f(1, 0), sf::Vector2f(720, 360), sf::Vector2f(760, 360), sf::Vector2f(760, 400), sf::Vector2f(720, 400) };
	floor[17] = { 1, sf::Vector2f(0, 1), sf::Vector2f(760, 320), sf::Vector2f(800, 320), sf::Vector2f(800, 360), sf::Vector2f(760, 360) };
	floor[18] = { 1, sf::Vector2f(0, 1), sf::Vector2f(760, 360), sf::Vector2f(800, 360), sf::Vector2f(800, 400), sf::Vector2f(760, 400) };
	floor[19] = { 1, sf::Vector2f(0, 1), sf::Vector2f(800, 320), sf::Vector2f(840, 320), sf::Vector2f(840, 360), sf::Vector2f(800, 360) };
	floor[20] = { 1, sf::Vector2f(0, 1), sf::Vector2f(800, 360), sf::Vector2f(840, 360), sf::Vector2f(840, 400), sf::Vector2f(800, 400) };
	floor[21] = { 1, sf::Vector2f(0, 1), sf::Vector2f(760, 400), sf::Vector2f(800, 400), sf::Vector2f(800, 440), sf::Vector2f(760, 440) };
	floor[22] = { 1, sf::Vector2f(0, 1), sf::Vector2f(800, 400), sf::Vector2f(840, 400), sf::Vector2f(840, 440), sf::Vector2f(800, 440) };
	floor[23] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(760, 440), sf::Vector2f(800, 440), sf::Vector2f(800, 480), sf::Vector2f(760, 480) };
	floor[24] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(800, 440), sf::Vector2f(840, 440), sf::Vector2f(840, 480), sf::Vector2f(800, 480) };
	floor[25] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(760, 480), sf::Vector2f(800, 480), sf::Vector2f(800, 520), sf::Vector2f(760, 520) };
	floor[26] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(800, 480), sf::Vector2f(840, 480), sf::Vector2f(840, 520), sf::Vector2f(800, 520) };
	floor[27] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(720, 440), sf::Vector2f(760, 440), sf::Vector2f(760, 480), sf::Vector2f(720, 480) };
	floor[28] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(720, 480), sf::Vector2f(760, 480), sf::Vector2f(760, 520), sf::Vector2f(720, 520) };
	floor[29] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(680, 440), sf::Vector2f(720, 440), sf::Vector2f(720, 480), sf::Vector2f(680, 480) };
	floor[30] = { 1, sf::Vector2f(-1, 0), sf::Vector2f(680, 480), sf::Vector2f(720, 480), sf::Vector2f(720, 520), sf::Vector2f(680, 520) };
	floor[31] = { 1, sf::Vector2f(0, 1), sf::Vector2f(640, 440), sf::Vector2f(680, 440), sf::Vector2f(680, 480), sf::Vector2f(640, 480) };
	floor[32] = { 1, sf::Vector2f(0, 1), sf::Vector2f(640, 480), sf::Vector2f(680, 480), sf::Vector2f(680, 520), sf::Vector2f(640, 520) };
	floor[33] = { 1, sf::Vector2f(0, 1), sf::Vector2f(600, 440), sf::Vector2f(640, 440), sf::Vector2f(640, 480), sf::Vector2f(600, 480) };
	floor[34] = { 1, sf::Vector2f(0, 1), sf::Vector2f(600, 480), sf::Vector2f(640, 480), sf::Vector2f(640, 520), sf::Vector2f(600, 520) };
	floor[35] = { 1, sf::Vector2f(0, 1), sf::Vector2f(600, 520), sf::Vector2f(640, 520), sf::Vector2f(640, 560), sf::Vector2f(600, 560) };
	floor[36] = { 1, sf::Vector2f(0, 1), sf::Vector2f(640, 520), sf::Vector2f(680, 520), sf::Vector2f(680, 560), sf::Vector2f(640, 560) };
	floor[37] = { 1, sf::Vector2f(1, 0), sf::Vector2f(600, 560), sf::Vector2f(640, 560), sf::Vector2f(640, 600), sf::Vector2f(600, 600) };
	floor[38] = { 1, sf::Vector2f(1, 0), sf::Vector2f(600, 600), sf::Vector2f(640, 600), sf::Vector2f(640, 640), sf::Vector2f(600, 640) };
	floor[39] = { 1, sf::Vector2f(1, 0), sf::Vector2f(640, 560), sf::Vector2f(680, 560), sf::Vector2f(680, 600), sf::Vector2f(640, 600) };
	floor[40] = { 1, sf::Vector2f(1, 0), sf::Vector2f(640, 600), sf::Vector2f(680, 600), sf::Vector2f(680, 640), sf::Vector2f(640, 640) };
	floor[41] = { 1, sf::Vector2f(1, 0), sf::Vector2f(680, 560), sf::Vector2f(720, 560), sf::Vector2f(720, 600), sf::Vector2f(680, 600) };
	floor[42] = { 1, sf::Vector2f(1, 0), sf::Vector2f(680, 600), sf::Vector2f(720, 600), sf::Vector2f(720, 640), sf::Vector2f(680, 640) };
	floor[43] = { 1, sf::Vector2f(1, 0), sf::Vector2f(720, 560), sf::Vector2f(760, 560), sf::Vector2f(760, 600), sf::Vector2f(720, 600) };
	floor[44] = { 1, sf::Vector2f(1, 0), sf::Vector2f(720, 600), sf::Vector2f(760, 600), sf::Vector2f(760, 640), sf::Vector2f(720, 640) };
	floor[45] = { 1, sf::Vector2f(1, 0), sf::Vector2f(760, 560), sf::Vector2f(800, 560), sf::Vector2f(800, 600), sf::Vector2f(760, 600) };
	floor[46] = { 1, sf::Vector2f(1, 0), sf::Vector2f(760, 600), sf::Vector2f(800, 600), sf::Vector2f(800, 640), sf::Vector2f(760, 640) };
	floor[47] = { 1, sf::Vector2f(1, 0), sf::Vector2f(800, 560), sf::Vector2f(840, 560), sf::Vector2f(840, 600), sf::Vector2f(800, 600) };
	floor[48] = { 1, sf::Vector2f(1, 0), sf::Vector2f(800, 600), sf::Vector2f(840, 600), sf::Vector2f(840, 640), sf::Vector2f(800, 640) };
	floor[49] = { 1, sf::Vector2f(1, 0), sf::Vector2f(840, 560), sf::Vector2f(880, 560), sf::Vector2f(880, 600), sf::Vector2f(840, 600) };
	floor[50] = { 1, sf::Vector2f(1, 0), sf::Vector2f(840, 600), sf::Vector2f(880, 600), sf::Vector2f(880, 640), sf::Vector2f(840, 640) };
	floor[51] = { 1, sf::Vector2f(1, 0), sf::Vector2f(600, 640), sf::Vector2f(640, 640), sf::Vector2f(640, 680), sf::Vector2f(600, 680) };
	floor[52] = { 1, sf::Vector2f(1, 0), sf::Vector2f(640, 640), sf::Vector2f(680, 640), sf::Vector2f(680, 680), sf::Vector2f(640, 680) };
	floor[53] = { 1, sf::Vector2f(1, 0), sf::Vector2f(680, 640), sf::Vector2f(720, 640), sf::Vector2f(720, 680), sf::Vector2f(680, 680) };
	floor[54] = { 1, sf::Vector2f(1, 0), sf::Vector2f(720, 640), sf::Vector2f(760, 640), sf::Vector2f(760, 680), sf::Vector2f(720, 680) };
	floor[55] = { 1, sf::Vector2f(1, 0), sf::Vector2f(760, 640), sf::Vector2f(800, 640), sf::Vector2f(800, 680), sf::Vector2f(760, 680) };
	floor[56] = { 1, sf::Vector2f(1, 0), sf::Vector2f(800, 640), sf::Vector2f(840, 640), sf::Vector2f(840, 680), sf::Vector2f(800, 680) };
	floor[57] = { 1, sf::Vector2f(1, 0), sf::Vector2f(840, 640), sf::Vector2f(880, 640), sf::Vector2f(880, 680), sf::Vector2f(840, 680) };

	//INITIALIZING PLAYERS

	Sphere ball1 = { {300, 100}, {0, 0}, {0, 0}, 20, 1, 255, 0 ,0 };
	Sphere ball2 = { {350, 100}, {0, 0}, {0, 0}, 20, 1, 0, 0 , 255 };

	Player players[2] = { {"RED", ball1, 0, sf::Vector2f(100, 600), 75, 255, 0, 0, sf::Vector2f(100, 450), 100, false}, {"BLUE", ball2, 0, sf::Vector2f(1180, 600), 75, 0, 0, 255, sf::Vector2f(1180, 450), 100, false} };
	
	sf::Vector2f curpos(0, 0);
	float t = 0;
	int move = 0;
	bool playEnd = false;

	//INITIALIZING HOLE
	Sphere Hole = { {1000, 100}, {0, 0}, {0, 0}, 20, 1, 0, 0, 0 };

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if ((!players[move].play) && isBallAtRest(players[0].ball) && isBallAtRest(players[1].ball) && isCursorOnController(curpos, players[move]))
				players[move].play = true;
			}
			if (event.type == sf::Event::MouseMoved)
			{
				curpos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (players[move].play)
				{
					players[move].play = false;
					players[move].ball.speed = players[move].direction(curpos) * players[move].force(t);
					players[move].count++;
					t = 0;
					move = (move + 1) % 2;
				}
			}
			if ((event.type == sf::Event::KeyPressed) || (event.type == sf::Event::MouseButtonPressed))
			{
				if (playEnd)
				{
					window.close();
					break;
				}
			}
		}

		if (players[0].count > players[1].count)
				endPlay(&window, players[0], &font);
			else
				endPlay(&window, players[1], &font);
		}
		else
		{
			if (players[move].play)
			{
				t += DT;
			}

			window.clear(sf::Color(50, 205, 50));

			for (int i = 0; i < 11; i++)
			{
				window.draw(walls[i]);
			}

			for (int i = 0; i < 58; i++)
			{
				floor[i].draw(&window);
			}

			players[0].draw(&window, curpos, t, &font);
			players[1].draw(&window, curpos, t, &font);

			Hole.draw(&window);
			window.display();

			if (players[0].ball.checkCollisionTwoSpheres(&players[1].ball))
				players[0].ball.collideSpheres(&players[1].ball, &window);

			if (players[0].ball.checkCollisionTwoSpheres(&Hole) || players[1].ball.checkCollisionTwoSpheres(&Hole))
			{
				playEnd = true;
			}

			players[0].ball.collideWithWalls(walls, DT);
			players[1].ball.collideWithWalls(walls, DT);

			if (pow(players[0].ball.speed.x, 2) + pow(players[0].ball.speed.y, 2) != 0)
				players[0].ball.friction(floor, N);
			if (pow(players[1].ball.speed.x, 2) + pow(players[1].ball.speed.y, 2) != 0)
				players[1].ball.friction(floor, N);

			players[0].ball.move(DT);
			players[1].ball.move(DT);
		}
	}

	return 0;
}
