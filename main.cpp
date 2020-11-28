#include <SFML/Graphics.hpp>
#include "Objects.h"
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

struct qwer
{
	sf::Vector2f pos;
	float radius;
	int red;
	int green;
	int blue;
	bool play;

	qwer(sf::Vector2f pos, float radius, int red, int green, int blue, bool play)
	{
		this->radius = radius;
		this->pos = pos;
		this->red = red;
		this->green = green;
		this->blue = blue;
		this->play = play;
	}

	sf::Vector2f direction(sf::Vector2f posCursor)
	{
		float length = sqrt(pow((posCursor - pos).x, 2) + pow((posCursor - pos).y, 2));
		sf::Vector2f dir(0, 0);
		if (length != 0)
		{
			dir = (posCursor - pos) / length;
		}
		return dir;
	}

	float force(float t)
	{
		return (1000 - abs(int(t * 10) % 2001 - 1000)) / 200.f;
	}

	void draw(sf::RenderWindow* window)
	{
		sf::CircleShape circle1(0, 30);
		circle1.setRadius(radius);
		circle1.setFillColor(sf::Color(red, green, blue));
		circle1.setPosition(pos - (sf::Vector2f(1, 1) * radius));
		sf::CircleShape circle2(0, 30);
		circle2.setRadius(radius / 4);
		circle2.setFillColor(sf::Color(red / 2, green / 2, blue / 2));
		circle2.setPosition(pos - (sf::Vector2f(1, 1) * radius / float(4)));

		window->draw(circle1);
		window->draw(circle2);
	}

};

void drawQwer(sf::RenderWindow* window, qwer Qwer, sf::Vector2f posBall, sf::Vector2f posCursor, float t)
{
	sf::Vector2f dir = Qwer.direction(posCursor);

	sf::Vector2f ort(-dir.y, dir.x);
	float length = sqrt(pow(ort.x, 2) + pow(ort.y, 2));
	ort = ort / length;

	sf::Vector2f A = posBall + ort * float(3);
	sf::Vector2f D = posBall - ort * float(3);
	sf::Vector2f B = posBall + dir * float(100) + ort * float(3);
	sf::Vector2f C = posBall + dir * float(100) - ort * float(3);
	sf::Vector2f E = B + ort * float(3);
	sf::Vector2f F = C - ort * float(3);
	sf::Vector2f G = posBall + dir * float(120);
	float force = Qwer.force(t) * 20;
	sf::Vector2f B1 = posBall + dir * float(force) + ort * float(3);
	sf::Vector2f C1 = posBall + dir * float(force) - ort * float(3);

	sf::ConvexShape arrow1;
	sf::ConvexShape arrow2;
	sf::ConvexShape arrow3;
	arrow1.setPointCount(4);
	arrow2.setPointCount(3);
	arrow3.setPointCount(4);
	arrow1.setPoint(0, A);
	arrow1.setPoint(1, B);
	arrow1.setPoint(2, C);
	arrow1.setPoint(3, D);

	arrow2.setPoint(0, E);
	arrow2.setPoint(1, G);
	arrow2.setPoint(2, F);

	arrow3.setPoint(0, A);
	arrow3.setPoint(1, B1);
	arrow3.setPoint(2, C1);
	arrow3.setPoint(3, D);

	arrow1.setFillColor(sf::Color(14, 241, 255));
	arrow2.setFillColor(sf::Color(14, 241, 255));
	arrow3.setFillColor(sf::Color(0, 0, 255));

	sf::CircleShape circle1(0, 30);
	circle1.setRadius(Qwer.radius);
	circle1.setFillColor(sf::Color(Qwer.red, Qwer.green, Qwer.blue));
	circle1.setPosition(Qwer.pos - (sf::Vector2f(1, 1) * Qwer.radius));
	sf::CircleShape circle2(0, 30);
	circle2.setRadius(Qwer.radius / 4);
	circle2.setFillColor(sf::Color(Qwer.red / 2, Qwer.green / 2, Qwer.blue / 2));
	circle2.setPosition(Qwer.pos - (sf::Vector2f(1, 1) * Qwer.radius / float(4)));

	window->draw(circle1);
	window->draw(circle2);
	window->draw(arrow1);
	window->draw(arrow2);
	window->draw(arrow3);
};

void drawShots(sf::RenderWindow* window, sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D, int count, sf::Font* font)
{
	sf::ConvexShape shape;
	shape.setPointCount(4);
	shape.setPoint(0, A);
	shape.setPoint(1, B);
	shape.setPoint(2, C);
	shape.setPoint(3, D);
	shape.setFillColor(sf::Color(0, 0, 0));

	sf::Text text("S H O T S", *font, 15);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setPosition(A + (D - A) / float(4) + (B - A) / float(10));

	std::string str = std::to_string(count);
	sf::Text text1(str, *font, 30);
	text1.setFillColor(sf::Color(255, 0, 0));
	text1.setPosition(A + (D - A) * float(3) / float(5) + (B - A) * float(2) / float(5));

	window->draw(shape);
	window->draw(text);
	window->draw(text1);
}

const int window_length = 1280;
const int window_width = 720;
const float DT = 1.0;

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
	sf::Vector2f curpos(0, 0);
	float t = 0;
	qwer Qwer(sf::Vector2f(100, 600), 75, 255, 0, 0, false);

	//INITIALIZING SAND AND BUSTERS

	const int N = 58;
	Floor floor[N];

	floor[0].def(-1, sf::Vector2f(0, 0), sf::Vector2f(320, 480), sf::Vector2f(400, 480), sf::Vector2f(400, 560), sf::Vector2f(320, 560));
	floor[1].def(1, sf::Vector2f(0, -1), sf::Vector2f(440, 480), sf::Vector2f(480, 480), sf::Vector2f(480, 520), sf::Vector2f(440, 520));
	floor[2].def(1, sf::Vector2f(0, -1), sf::Vector2f(480, 480), sf::Vector2f(520, 480), sf::Vector2f(520, 520), sf::Vector2f(480, 520));
	floor[3].def(1, sf::Vector2f(0, -1), sf::Vector2f(520, 480), sf::Vector2f(560, 480), sf::Vector2f(560, 520), sf::Vector2f(520, 520));
	floor[4].def(1, sf::Vector2f(0, -1), sf::Vector2f(440, 520), sf::Vector2f(480, 520), sf::Vector2f(480, 560), sf::Vector2f(440, 560));
	floor[5].def(1, sf::Vector2f(0, -1), sf::Vector2f(480, 520), sf::Vector2f(520, 520), sf::Vector2f(520, 560), sf::Vector2f(480, 560));
	floor[6].def(1, sf::Vector2f(0, -1), sf::Vector2f(520, 520), sf::Vector2f(560, 520), sf::Vector2f(560, 560), sf::Vector2f(520, 560));
	floor[7].def(1, sf::Vector2f(0, 1), sf::Vector2f(600, 280), sf::Vector2f(640, 280), sf::Vector2f(640, 320), sf::Vector2f(600, 320));
	floor[8].def(1, sf::Vector2f(0, 1), sf::Vector2f(640, 280), sf::Vector2f(680, 280), sf::Vector2f(680, 320), sf::Vector2f(640, 320));
	floor[9].def(1, sf::Vector2f(1, 0), sf::Vector2f(600, 320), sf::Vector2f(640, 320), sf::Vector2f(640, 360), sf::Vector2f(600, 360));
	floor[10].def(1, sf::Vector2f(1, 0), sf::Vector2f(600, 360), sf::Vector2f(640, 360), sf::Vector2f(640, 400), sf::Vector2f(600, 400));
	floor[11].def(1, sf::Vector2f(1, 0), sf::Vector2f(640, 320), sf::Vector2f(680, 320), sf::Vector2f(680, 360), sf::Vector2f(640, 360));
	floor[12].def(1, sf::Vector2f(1, 0), sf::Vector2f(640, 360), sf::Vector2f(680, 360), sf::Vector2f(680, 400), sf::Vector2f(640, 400));
	floor[13].def(1, sf::Vector2f(1, 0), sf::Vector2f(680, 320), sf::Vector2f(720, 320), sf::Vector2f(720, 360), sf::Vector2f(680, 360));
	floor[14].def(1, sf::Vector2f(1, 0), sf::Vector2f(680, 360), sf::Vector2f(720, 360), sf::Vector2f(720, 400), sf::Vector2f(680, 400));
	floor[15].def(1, sf::Vector2f(1, 0), sf::Vector2f(720, 320), sf::Vector2f(760, 320), sf::Vector2f(760, 360), sf::Vector2f(720, 360));
	floor[16].def(1, sf::Vector2f(1, 0), sf::Vector2f(720, 360), sf::Vector2f(760, 360), sf::Vector2f(760, 400), sf::Vector2f(720, 400));
	floor[17].def(1, sf::Vector2f(0, 1), sf::Vector2f(760, 320), sf::Vector2f(800, 320), sf::Vector2f(800, 360), sf::Vector2f(760, 360));
	floor[18].def(1, sf::Vector2f(0, 1), sf::Vector2f(760, 360), sf::Vector2f(800, 360), sf::Vector2f(800, 400), sf::Vector2f(760, 400));
	floor[19].def(1, sf::Vector2f(0, 1), sf::Vector2f(800, 320), sf::Vector2f(840, 320), sf::Vector2f(840, 360), sf::Vector2f(800, 360));
	floor[20].def(1, sf::Vector2f(0, 1), sf::Vector2f(800, 360), sf::Vector2f(840, 360), sf::Vector2f(840, 400), sf::Vector2f(800, 400));
	floor[21].def(1, sf::Vector2f(0, 1), sf::Vector2f(760, 400), sf::Vector2f(800, 400), sf::Vector2f(800, 440), sf::Vector2f(760, 440));
	floor[22].def(1, sf::Vector2f(0, 1), sf::Vector2f(800, 400), sf::Vector2f(840, 400), sf::Vector2f(840, 440), sf::Vector2f(800, 440));
	floor[23].def(1, sf::Vector2f(-1, 0), sf::Vector2f(760, 440), sf::Vector2f(800, 440), sf::Vector2f(800, 480), sf::Vector2f(760, 480));
	floor[24].def(1, sf::Vector2f(-1, 0), sf::Vector2f(800, 440), sf::Vector2f(840, 440), sf::Vector2f(840, 480), sf::Vector2f(800, 480));
	floor[25].def(1, sf::Vector2f(-1, 0), sf::Vector2f(760, 480), sf::Vector2f(800, 480), sf::Vector2f(800, 520), sf::Vector2f(760, 520));
	floor[26].def(1, sf::Vector2f(-1, 0), sf::Vector2f(800, 480), sf::Vector2f(840, 480), sf::Vector2f(840, 520), sf::Vector2f(800, 520));
	floor[27].def(1, sf::Vector2f(-1, 0), sf::Vector2f(720, 440), sf::Vector2f(760, 440), sf::Vector2f(760, 480), sf::Vector2f(720, 480));
	floor[28].def(1, sf::Vector2f(-1, 0), sf::Vector2f(720, 480), sf::Vector2f(760, 480), sf::Vector2f(760, 520), sf::Vector2f(720, 520));
	floor[29].def(1, sf::Vector2f(-1, 0), sf::Vector2f(680, 440), sf::Vector2f(720, 440), sf::Vector2f(720, 480), sf::Vector2f(680, 480));
	floor[30].def(1, sf::Vector2f(-1, 0), sf::Vector2f(680, 480), sf::Vector2f(720, 480), sf::Vector2f(720, 520), sf::Vector2f(680, 520));
	floor[31].def(1, sf::Vector2f(0, 1), sf::Vector2f(640, 440), sf::Vector2f(680, 440), sf::Vector2f(680, 480), sf::Vector2f(640, 480));
	floor[32].def(1, sf::Vector2f(0, 1), sf::Vector2f(640, 480), sf::Vector2f(680, 480), sf::Vector2f(680, 520), sf::Vector2f(640, 520));
	floor[33].def(1, sf::Vector2f(0, 1), sf::Vector2f(600, 440), sf::Vector2f(640, 440), sf::Vector2f(640, 480), sf::Vector2f(600, 480));
	floor[34].def(1, sf::Vector2f(0, 1), sf::Vector2f(600, 480), sf::Vector2f(640, 480), sf::Vector2f(640, 520), sf::Vector2f(600, 520));
	floor[35].def(1, sf::Vector2f(0, 1), sf::Vector2f(600, 520), sf::Vector2f(640, 520), sf::Vector2f(640, 560), sf::Vector2f(600, 560));
	floor[36].def(1, sf::Vector2f(0, 1), sf::Vector2f(640, 520), sf::Vector2f(680, 520), sf::Vector2f(680, 560), sf::Vector2f(640, 560));
	floor[37].def(1, sf::Vector2f(1, 0), sf::Vector2f(600, 560), sf::Vector2f(640, 560), sf::Vector2f(640, 600), sf::Vector2f(600, 600));
	floor[38].def(1, sf::Vector2f(1, 0), sf::Vector2f(600, 600), sf::Vector2f(640, 600), sf::Vector2f(640, 640), sf::Vector2f(600, 640));
	floor[39].def(1, sf::Vector2f(1, 0), sf::Vector2f(640, 560), sf::Vector2f(680, 560), sf::Vector2f(680, 600), sf::Vector2f(640, 600));
	floor[40].def(1, sf::Vector2f(1, 0), sf::Vector2f(640, 600), sf::Vector2f(680, 600), sf::Vector2f(680, 640), sf::Vector2f(640, 640));
	floor[41].def(1, sf::Vector2f(1, 0), sf::Vector2f(680, 560), sf::Vector2f(720, 560), sf::Vector2f(720, 600), sf::Vector2f(680, 600));
	floor[42].def(1, sf::Vector2f(1, 0), sf::Vector2f(680, 600), sf::Vector2f(720, 600), sf::Vector2f(720, 640), sf::Vector2f(680, 640));
	floor[43].def(1, sf::Vector2f(1, 0), sf::Vector2f(720, 560), sf::Vector2f(760, 560), sf::Vector2f(760, 600), sf::Vector2f(720, 600));
	floor[44].def(1, sf::Vector2f(1, 0), sf::Vector2f(720, 600), sf::Vector2f(760, 600), sf::Vector2f(760, 640), sf::Vector2f(720, 640));
	floor[45].def(1, sf::Vector2f(1, 0), sf::Vector2f(760, 560), sf::Vector2f(800, 560), sf::Vector2f(800, 600), sf::Vector2f(760, 600));
	floor[46].def(1, sf::Vector2f(1, 0), sf::Vector2f(760, 600), sf::Vector2f(800, 600), sf::Vector2f(800, 640), sf::Vector2f(760, 640));
	floor[47].def(1, sf::Vector2f(1, 0), sf::Vector2f(800, 560), sf::Vector2f(840, 560), sf::Vector2f(840, 600), sf::Vector2f(800, 600));
	floor[48].def(1, sf::Vector2f(1, 0), sf::Vector2f(800, 600), sf::Vector2f(840, 600), sf::Vector2f(840, 640), sf::Vector2f(800, 640));
	floor[49].def(1, sf::Vector2f(1, 0), sf::Vector2f(840, 560), sf::Vector2f(880, 560), sf::Vector2f(880, 600), sf::Vector2f(840, 600));
	floor[50].def(1, sf::Vector2f(1, 0), sf::Vector2f(840, 600), sf::Vector2f(880, 600), sf::Vector2f(880, 640), sf::Vector2f(840, 640));
	floor[51].def(1, sf::Vector2f(1, 0), sf::Vector2f(600, 640), sf::Vector2f(640, 640), sf::Vector2f(640, 680), sf::Vector2f(600, 680));
	floor[52].def(1, sf::Vector2f(1, 0), sf::Vector2f(640, 640), sf::Vector2f(680, 640), sf::Vector2f(680, 680), sf::Vector2f(640, 680));
	floor[53].def(1, sf::Vector2f(1, 0), sf::Vector2f(680, 640), sf::Vector2f(720, 640), sf::Vector2f(720, 680), sf::Vector2f(680, 680));
	floor[54].def(1, sf::Vector2f(1, 0), sf::Vector2f(720, 640), sf::Vector2f(760, 640), sf::Vector2f(760, 680), sf::Vector2f(720, 680));
	floor[55].def(1, sf::Vector2f(1, 0), sf::Vector2f(760, 640), sf::Vector2f(800, 640), sf::Vector2f(800, 680), sf::Vector2f(760, 680));
	floor[56].def(1, sf::Vector2f(1, 0), sf::Vector2f(800, 640), sf::Vector2f(840, 640), sf::Vector2f(840, 680), sf::Vector2f(800, 680));
	floor[57].def(1, sf::Vector2f(1, 0), sf::Vector2f(840, 640), sf::Vector2f(880, 640), sf::Vector2f(880, 680), sf::Vector2f(840, 680));
	//INITIALIZING PLAYERS

	Sphere ball1 = { {900, 100}, {0, 0}, {0, 0}, 20, 1, 255, 0 ,0 };
	int count = 0;

	//INITIALIZING HOLE
	Sphere Hole = { {1000, 100}, {0, 0}, {0, 0}, 20, 1, 0, 0, 0 };

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) || (event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::MouseButtonPressed && (pow(curpos.x - Qwer.pos.x, 2) + pow(curpos.y - Qwer.pos.y, 2) < pow(Qwer.radius, 2)))
			{
				Qwer.play = true;
			}
			if (event.type == sf::Event::MouseMoved)
			{
				curpos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
			}
			if (event.type == sf::Event::MouseButtonReleased && Qwer.play)
			{
				Qwer.play = false;
				ball1.speed = Qwer.direction(curpos) * Qwer.force(t);
				count++;
				t = 0;
			}
		}

		if (Qwer.play)
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

		if (Qwer.play)
		{
			drawQwer(&window, Qwer, ball1.pos, curpos, t);
		}
		else
		{
			Qwer.draw(&window);
		};

		drawShots(&window, sf::Vector2f(50, 400), sf::Vector2f(150, 400), sf::Vector2f(150, 500), sf::Vector2f(50, 500), count, &font);
		ball1.draw(&window);
		Hole.draw(&window);
		window.display();

		if (ball1.checkCollisionTwoSpheres(&Hole))
		{
			window.clear(sf::Color(255, 0, 0));

			sf::Text wincongrat(std::string("RED PLAYER WON IN ") + std::to_string(count) + std::string(" MOVES"), font, 50);
			wincongrat.setFillColor(sf::Color(255, 255, 255));
			wincongrat.setPosition(250, 335);

			window.draw(wincongrat);

			window.display();
			
			while (true)
			{
				window.pollEvent(event);
				if ((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed) || (event.type == sf::Event::MouseButtonPressed))
				{
					window.close();
					break;
				}
			}
		}

		for (int i = 0; i < 11; i++)
		{
			ball1.collide(&walls[i], DT);
		}
		ball1.flag = false;

		if (pow(ball1.speed.x, 2) + pow(ball1.speed.y, 2) != 0)
			ball1.friction(floor, N);

		ball1.move(DT);
	}

	return 0;
}
