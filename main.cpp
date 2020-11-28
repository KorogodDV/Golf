#include <SFML/Graphics.hpp>
#include "Objects.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>



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
		return (1000 - abs(int(t * 10) % 2000 - 1000)) / 400;
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
	
	sf::Vector2f ort(dir.y, dir.x);
	if (dir.y != 0)
	{
		sf::Vector2f ort(1, - dir.x / dir.y);
		float length = sqrt(pow(ort.x, 2) + pow(ort.y, 2));
		ort = ort / length;
	}
	
	sf::Vector2f A = posBall + ort * float(3);
	sf::Vector2f D = posBall - ort * float(3);
	sf::Vector2f B = posBall + dir * float(100) + ort * float(3);
	sf::Vector2f C = posBall + dir * float(100) - ort * float(3);
	sf::Vector2f E = B + ort * float(3);
	sf::Vector2f F = C - ort * float(3);
	sf::Vector2f G = posBall + dir * float(120);
	float force = Qwer.force(t) * 40;
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

	arrow1.setFillColor(sf::Color(255, 255, 0));
	arrow2.setFillColor(sf::Color(255, 255, 0));
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

void drawShots(sf::RenderWindow* window, sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D, int count)
{
	sf::Font font;
	int sizeFont = 10;
	font.loadFromFile("C:\\Users\\Дмитрий\\Documents\\19924.ttf");

	sf::ConvexShape shape;
	shape.setPointCount(4);
	shape.setPoint(0, A);
	shape.setPoint(1, B);
	shape.setPoint(2, C);
	shape.setPoint(3, D);
	shape.setFillColor(sf::Color(0, 0, 0));

	sf::Text text("S H O T S", font, 15);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setPosition(A + (D - A) / float(4) + (B - A) / float(10));

	std::string str = std::to_string(count);
	sf::Text text1(str, font, 30);
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

	sf::RenderWindow window(sf::VideoMode(window_length, window_width), "Golf");
	window.setFramerateLimit(60);
	
	//INTIALIZING WALLS

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

	sf::Vector2f curpos(0, 0);
	float t = 0;
	qwer Qwer(sf::Vector2f(100, 600), 75, 255, 0, 0, false);
	
	//INITIALIZING SAND AND BUSTERS

	const int N = 58;
	Floor floor[N];

	floor[0].mu = -1;
	floor[0].rect.setPoint(0, sf::Vector2f(320, 480));
	floor[0].rect.setPoint(1, sf::Vector2f(400, 480));
	floor[0].rect.setPoint(2, sf::Vector2f(400, 560));
	floor[0].rect.setPoint(3, sf::Vector2f(320, 560));

	floor[1].mu = 1;
	floor[1].direction = sf::Vector2f(0, -1);
	floor[1].rect.setPoint(0, sf::Vector2f(440, 480));
	floor[1].rect.setPoint(1, sf::Vector2f(480, 480));
	floor[1].rect.setPoint(2, sf::Vector2f(480, 520));
	floor[1].rect.setPoint(3, sf::Vector2f(440, 520));

	floor[2].mu = 1;
	floor[2].direction = sf::Vector2f(0, -1);
	floor[2].rect.setPoint(0, sf::Vector2f(480, 480));
	floor[2].rect.setPoint(1, sf::Vector2f(520, 480));
	floor[2].rect.setPoint(2, sf::Vector2f(520, 520));
	floor[2].rect.setPoint(3, sf::Vector2f(480, 520));

	floor[3].mu = 1;
	floor[3].direction = sf::Vector2f(0, -1);
	floor[3].rect.setPoint(0, sf::Vector2f(520, 480));
	floor[3].rect.setPoint(1, sf::Vector2f(560, 480));
	floor[3].rect.setPoint(2, sf::Vector2f(560, 520));
	floor[3].rect.setPoint(3, sf::Vector2f(520, 520));

	floor[4].mu = 1;
	floor[4].direction = sf::Vector2f(0, -1);
	floor[4].rect.setPoint(0, sf::Vector2f(440, 520));
	floor[4].rect.setPoint(1, sf::Vector2f(480, 520));
	floor[4].rect.setPoint(2, sf::Vector2f(480, 560));
	floor[4].rect.setPoint(3, sf::Vector2f(440, 560));

	floor[5].mu = 1;
	floor[5].direction = sf::Vector2f(0, -1);
	floor[5].rect.setPoint(0, sf::Vector2f(480, 520));
	floor[5].rect.setPoint(1, sf::Vector2f(520, 520));
	floor[5].rect.setPoint(2, sf::Vector2f(520, 560));
	floor[5].rect.setPoint(3, sf::Vector2f(480, 560));

	floor[6].mu = 1;
	floor[6].direction = sf::Vector2f(0, -1);
	floor[6].rect.setPoint(0, sf::Vector2f(520, 520));
	floor[6].rect.setPoint(1, sf::Vector2f(560, 520));
	floor[6].rect.setPoint(2, sf::Vector2f(560, 560));
	floor[6].rect.setPoint(3, sf::Vector2f(520, 560));

	floor[7].mu = 1;
	floor[7].direction = sf::Vector2f(0, 1);
	floor[7].rect.setPoint(0, sf::Vector2f(600, 280));
	floor[7].rect.setPoint(1, sf::Vector2f(640, 280));
	floor[7].rect.setPoint(2, sf::Vector2f(640, 320));
	floor[7].rect.setPoint(3, sf::Vector2f(600, 320));

	floor[8].mu = 1;
	floor[8].direction = sf::Vector2f(0, 1);
	floor[8].rect.setPoint(0, sf::Vector2f(640, 280));
	floor[8].rect.setPoint(1, sf::Vector2f(680, 280));
	floor[8].rect.setPoint(2, sf::Vector2f(680, 320));
	floor[8].rect.setPoint(3, sf::Vector2f(640, 320));

	floor[9].mu = 1;
	floor[9].direction = sf::Vector2f(1, 0);
	floor[9].rect.setPoint(0, sf::Vector2f(600, 320));
	floor[9].rect.setPoint(1, sf::Vector2f(640, 320));
	floor[9].rect.setPoint(2, sf::Vector2f(640, 360));
	floor[9].rect.setPoint(3, sf::Vector2f(600, 360));

	floor[10].mu = 1;
	floor[10].direction = sf::Vector2f(1, 0);
	floor[10].rect.setPoint(0, sf::Vector2f(600, 360));
	floor[10].rect.setPoint(1, sf::Vector2f(640, 360));
	floor[10].rect.setPoint(2, sf::Vector2f(640, 400));
	floor[10].rect.setPoint(3, sf::Vector2f(600, 400));

	floor[11].mu = 1;
	floor[11].direction = sf::Vector2f(1, 0);
	floor[11].rect.setPoint(0, sf::Vector2f(640, 320));
	floor[11].rect.setPoint(1, sf::Vector2f(680, 320));
	floor[11].rect.setPoint(2, sf::Vector2f(680, 360));
	floor[11].rect.setPoint(3, sf::Vector2f(640, 360));

	floor[12].mu = 1;
	floor[12].direction = sf::Vector2f(1, 0);
	floor[12].rect.setPoint(0, sf::Vector2f(640, 360));
	floor[12].rect.setPoint(1, sf::Vector2f(680, 360));
	floor[12].rect.setPoint(2, sf::Vector2f(680, 400));
	floor[12].rect.setPoint(3, sf::Vector2f(640, 400));

	floor[13].mu = 1;
	floor[13].direction = sf::Vector2f(1, 0);
	floor[13].rect.setPoint(0, sf::Vector2f(680, 320));
	floor[13].rect.setPoint(1, sf::Vector2f(720, 320));
	floor[13].rect.setPoint(2, sf::Vector2f(720, 360));
	floor[13].rect.setPoint(3, sf::Vector2f(680, 360));

	floor[14].mu = 1;
	floor[14].direction = sf::Vector2f(1, 0);
	floor[14].rect.setPoint(0, sf::Vector2f(680, 360));
	floor[14].rect.setPoint(1, sf::Vector2f(720, 360));
	floor[14].rect.setPoint(2, sf::Vector2f(720, 400));
	floor[14].rect.setPoint(3, sf::Vector2f(680, 400));

	floor[15].mu = 1;
	floor[15].direction = sf::Vector2f(1, 0);
	floor[15].rect.setPoint(0, sf::Vector2f(720, 320));
	floor[15].rect.setPoint(1, sf::Vector2f(760, 320));
	floor[15].rect.setPoint(2, sf::Vector2f(760, 360));
	floor[15].rect.setPoint(3, sf::Vector2f(720, 360));

	floor[16].mu = 1;
	floor[16].direction = sf::Vector2f(1, 0);
	floor[16].rect.setPoint(0, sf::Vector2f(720, 360));
	floor[16].rect.setPoint(1, sf::Vector2f(760, 360));
	floor[16].rect.setPoint(2, sf::Vector2f(760, 400));
	floor[16].rect.setPoint(3, sf::Vector2f(720, 400));

	floor[17].mu = 1;
	floor[17].direction = sf::Vector2f(0, 1);
	floor[17].rect.setPoint(0, sf::Vector2f(760, 320));
	floor[17].rect.setPoint(1, sf::Vector2f(800, 320));
	floor[17].rect.setPoint(2, sf::Vector2f(800, 360));
	floor[17].rect.setPoint(3, sf::Vector2f(760, 360));

	floor[18].mu = 1;
	floor[18].direction = sf::Vector2f(0, 1);
	floor[18].rect.setPoint(0, sf::Vector2f(760, 360));
	floor[18].rect.setPoint(1, sf::Vector2f(800, 360));
	floor[18].rect.setPoint(2, sf::Vector2f(800, 400));
	floor[18].rect.setPoint(3, sf::Vector2f(760, 400));

	floor[19].mu = 1;
	floor[19].direction = sf::Vector2f(0, 1);
	floor[19].rect.setPoint(0, sf::Vector2f(800, 320));
	floor[19].rect.setPoint(1, sf::Vector2f(840, 320));
	floor[19].rect.setPoint(2, sf::Vector2f(840, 360));
	floor[19].rect.setPoint(3, sf::Vector2f(800, 360));

	floor[20].mu = 1;
	floor[20].direction = sf::Vector2f(0, 1);
	floor[20].rect.setPoint(0, sf::Vector2f(800, 360));
	floor[20].rect.setPoint(1, sf::Vector2f(840, 360));
	floor[20].rect.setPoint(2, sf::Vector2f(840, 400));
	floor[20].rect.setPoint(3, sf::Vector2f(800, 400));

	floor[21].mu = 1;
	floor[21].direction = sf::Vector2f(0, 1);
	floor[21].rect.setPoint(0, sf::Vector2f(760, 400));
	floor[21].rect.setPoint(1, sf::Vector2f(800, 400));
	floor[21].rect.setPoint(2, sf::Vector2f(800, 440));
	floor[21].rect.setPoint(3, sf::Vector2f(760, 440));

	floor[22].mu = 1;
	floor[22].direction = sf::Vector2f(0, 1);
	floor[22].rect.setPoint(0, sf::Vector2f(800, 400));
	floor[22].rect.setPoint(1, sf::Vector2f(840, 400));
	floor[22].rect.setPoint(2, sf::Vector2f(840, 440));
	floor[22].rect.setPoint(3, sf::Vector2f(800, 440));

	floor[23].mu = 1;
	floor[23].direction = sf::Vector2f(-1, 0);
	floor[23].rect.setPoint(0, sf::Vector2f(760, 440));
	floor[23].rect.setPoint(1, sf::Vector2f(800, 440));
	floor[23].rect.setPoint(2, sf::Vector2f(800, 480));
	floor[23].rect.setPoint(3, sf::Vector2f(760, 480));

	floor[24].mu = 1;
	floor[24].direction = sf::Vector2f(-1, 0);
	floor[24].rect.setPoint(0, sf::Vector2f(800, 440));
	floor[24].rect.setPoint(1, sf::Vector2f(840, 440));
	floor[24].rect.setPoint(2, sf::Vector2f(840, 480));
	floor[24].rect.setPoint(3, sf::Vector2f(800, 480));

	floor[25].mu = 1;
	floor[25].direction = sf::Vector2f(-1, 0);
	floor[25].rect.setPoint(0, sf::Vector2f(760, 480));
	floor[25].rect.setPoint(1, sf::Vector2f(800, 480));
	floor[25].rect.setPoint(2, sf::Vector2f(800, 520));
	floor[25].rect.setPoint(3, sf::Vector2f(760, 520));

	floor[26].mu = 1;
	floor[26].direction = sf::Vector2f(-1, 0);
	floor[26].rect.setPoint(0, sf::Vector2f(800, 480));
	floor[26].rect.setPoint(1, sf::Vector2f(840, 480));
	floor[26].rect.setPoint(2, sf::Vector2f(840, 520));
	floor[26].rect.setPoint(3, sf::Vector2f(800, 520));

	floor[27].mu = 1;
	floor[27].direction = sf::Vector2f(-1, 0);
	floor[27].rect.setPoint(0, sf::Vector2f(720, 440));
	floor[27].rect.setPoint(1, sf::Vector2f(760, 440));
	floor[27].rect.setPoint(2, sf::Vector2f(760, 480));
	floor[27].rect.setPoint(3, sf::Vector2f(720, 480));

	floor[28].mu = 1;
	floor[28].direction = sf::Vector2f(-1, 0);
	floor[28].rect.setPoint(0, sf::Vector2f(720, 480));
	floor[28].rect.setPoint(1, sf::Vector2f(760, 480));
	floor[28].rect.setPoint(2, sf::Vector2f(760, 520));
	floor[28].rect.setPoint(3, sf::Vector2f(720, 520));

	floor[29].mu = 1;
	floor[29].direction = sf::Vector2f(-1, 0);
	floor[29].rect.setPoint(0, sf::Vector2f(680, 440));
	floor[29].rect.setPoint(1, sf::Vector2f(720, 440));
	floor[29].rect.setPoint(2, sf::Vector2f(720, 480));
	floor[29].rect.setPoint(3, sf::Vector2f(680, 480));

	floor[30].mu = 1;
	floor[30].direction = sf::Vector2f(-1, 0);
	floor[30].rect.setPoint(0, sf::Vector2f(680, 480));
	floor[30].rect.setPoint(1, sf::Vector2f(720, 480));
	floor[30].rect.setPoint(2, sf::Vector2f(720, 520));
	floor[30].rect.setPoint(3, sf::Vector2f(680, 520));

	floor[31].mu = 1;
	floor[31].direction = sf::Vector2f(0, 1);
	floor[31].rect.setPoint(0, sf::Vector2f(640, 440));
	floor[31].rect.setPoint(1, sf::Vector2f(680, 440));
	floor[31].rect.setPoint(2, sf::Vector2f(680, 480));
	floor[31].rect.setPoint(3, sf::Vector2f(640, 480));

	floor[32].mu = 1;
	floor[32].direction = sf::Vector2f(0, 1);
	floor[32].rect.setPoint(0, sf::Vector2f(640, 480));
	floor[32].rect.setPoint(1, sf::Vector2f(680, 480));
	floor[32].rect.setPoint(2, sf::Vector2f(680, 520));
	floor[32].rect.setPoint(3, sf::Vector2f(640, 520));

	floor[33].mu = 1;
	floor[33].direction = sf::Vector2f(0, 1);
	floor[33].rect.setPoint(0, sf::Vector2f(600, 440));
	floor[33].rect.setPoint(1, sf::Vector2f(640, 440));
	floor[33].rect.setPoint(2, sf::Vector2f(640, 480));
	floor[33].rect.setPoint(3, sf::Vector2f(600, 480));

	floor[34].mu = 1;
	floor[34].direction = sf::Vector2f(0, 1);
	floor[34].rect.setPoint(0, sf::Vector2f(600, 480));
	floor[34].rect.setPoint(1, sf::Vector2f(640, 480));
	floor[34].rect.setPoint(2, sf::Vector2f(640, 520));
	floor[34].rect.setPoint(3, sf::Vector2f(600, 520));

	floor[35].mu = 1;
	floor[35].direction = sf::Vector2f(0, 1);
	floor[35].rect.setPoint(0, sf::Vector2f(600, 520));
	floor[35].rect.setPoint(1, sf::Vector2f(640, 520));
	floor[35].rect.setPoint(2, sf::Vector2f(640, 560));
	floor[35].rect.setPoint(3, sf::Vector2f(600, 560));

	floor[36].mu = 1;
	floor[36].direction = sf::Vector2f(0, 1);
	floor[36].rect.setPoint(0, sf::Vector2f(640, 520));
	floor[36].rect.setPoint(1, sf::Vector2f(680, 520));
	floor[36].rect.setPoint(2, sf::Vector2f(680, 560));
	floor[36].rect.setPoint(3, sf::Vector2f(640, 560));

	floor[37].mu = 1;
	floor[37].direction = sf::Vector2f(1, 0);
	floor[37].rect.setPoint(0, sf::Vector2f(600, 560));
	floor[37].rect.setPoint(1, sf::Vector2f(640, 560));
	floor[37].rect.setPoint(2, sf::Vector2f(640, 600));
	floor[37].rect.setPoint(3, sf::Vector2f(600, 600));

	floor[38].mu = 1;
	floor[38].direction = sf::Vector2f(1, 0);
	floor[38].rect.setPoint(0, sf::Vector2f(600, 600));
	floor[38].rect.setPoint(1, sf::Vector2f(640, 600));
	floor[38].rect.setPoint(2, sf::Vector2f(640, 640));
	floor[38].rect.setPoint(3, sf::Vector2f(600, 640));

	floor[39].mu = 1;
	floor[39].direction = sf::Vector2f(1, 0);
	floor[39].rect.setPoint(0, sf::Vector2f(640, 560));
	floor[39].rect.setPoint(1, sf::Vector2f(680, 560));
	floor[39].rect.setPoint(2, sf::Vector2f(680, 600));
	floor[39].rect.setPoint(3, sf::Vector2f(640, 600));

	floor[40].mu = 1;
	floor[40].direction = sf::Vector2f(1, 0);
	floor[40].rect.setPoint(0, sf::Vector2f(640, 600));
	floor[40].rect.setPoint(1, sf::Vector2f(680, 600));
	floor[40].rect.setPoint(2, sf::Vector2f(680, 640));
	floor[40].rect.setPoint(3, sf::Vector2f(640, 640));

	floor[41].mu = 1;
	floor[41].direction = sf::Vector2f(1, 0);
	floor[41].rect.setPoint(0, sf::Vector2f(680, 560));
	floor[41].rect.setPoint(1, sf::Vector2f(720, 560));
	floor[41].rect.setPoint(2, sf::Vector2f(720, 600));
	floor[41].rect.setPoint(3, sf::Vector2f(680, 600));

	floor[42].mu = 1;
	floor[42].direction = sf::Vector2f(1, 0);
	floor[42].rect.setPoint(0, sf::Vector2f(680, 600));
	floor[42].rect.setPoint(1, sf::Vector2f(720, 600));
	floor[42].rect.setPoint(2, sf::Vector2f(720, 640));
	floor[42].rect.setPoint(3, sf::Vector2f(680, 640));

	floor[43].mu = 1;
	floor[43].direction = sf::Vector2f(1, 0);
	floor[43].rect.setPoint(0, sf::Vector2f(720, 560));
	floor[43].rect.setPoint(1, sf::Vector2f(760, 560));
	floor[43].rect.setPoint(2, sf::Vector2f(760, 600));
	floor[43].rect.setPoint(3, sf::Vector2f(720, 600));

	floor[44].mu = 1;
	floor[44].direction = sf::Vector2f(1, 0);
	floor[44].rect.setPoint(0, sf::Vector2f(720, 600));
	floor[44].rect.setPoint(1, sf::Vector2f(760, 600));
	floor[44].rect.setPoint(2, sf::Vector2f(760, 640));
	floor[44].rect.setPoint(3, sf::Vector2f(720, 640));

	floor[45].mu = 1;
	floor[45].direction = sf::Vector2f(1, 0);
	floor[45].rect.setPoint(0, sf::Vector2f(760, 560));
	floor[45].rect.setPoint(1, sf::Vector2f(800, 560));
	floor[45].rect.setPoint(2, sf::Vector2f(800, 600));
	floor[45].rect.setPoint(3, sf::Vector2f(760, 600));

	floor[46].mu = 1;
	floor[46].direction = sf::Vector2f(1, 0);
	floor[46].rect.setPoint(0, sf::Vector2f(760, 600));
	floor[46].rect.setPoint(1, sf::Vector2f(800, 600));
	floor[46].rect.setPoint(2, sf::Vector2f(800, 640));
	floor[46].rect.setPoint(3, sf::Vector2f(760, 640));

	floor[47].mu = 1;
	floor[47].direction = sf::Vector2f(1, 0);
	floor[47].rect.setPoint(0, sf::Vector2f(800, 560));
	floor[47].rect.setPoint(1, sf::Vector2f(840, 560));
	floor[47].rect.setPoint(2, sf::Vector2f(840, 600));
	floor[47].rect.setPoint(3, sf::Vector2f(800, 600));

	floor[48].mu = 1;
	floor[48].direction = sf::Vector2f(1, 0);
	floor[48].rect.setPoint(0, sf::Vector2f(800, 600));
	floor[48].rect.setPoint(1, sf::Vector2f(840, 600));
	floor[48].rect.setPoint(2, sf::Vector2f(840, 640));
	floor[48].rect.setPoint(3, sf::Vector2f(800, 640));

	floor[49].mu = 1;
	floor[49].direction = sf::Vector2f(1, 0);
	floor[49].rect.setPoint(0, sf::Vector2f(840, 560));
	floor[49].rect.setPoint(1, sf::Vector2f(880, 560));
	floor[49].rect.setPoint(2, sf::Vector2f(880, 600));
	floor[49].rect.setPoint(3, sf::Vector2f(840, 600));

	floor[50].mu = 1;
	floor[50].direction = sf::Vector2f(1, 0);
	floor[50].rect.setPoint(0, sf::Vector2f(840, 600));
	floor[50].rect.setPoint(1, sf::Vector2f(880, 600));
	floor[50].rect.setPoint(2, sf::Vector2f(880, 640));
	floor[50].rect.setPoint(3, sf::Vector2f(840, 640));

	floor[51].mu = 1;
	floor[51].direction = sf::Vector2f(1, 0);
	floor[51].rect.setPoint(0, sf::Vector2f(600, 640));
	floor[51].rect.setPoint(1, sf::Vector2f(640, 640));
	floor[51].rect.setPoint(2, sf::Vector2f(640, 680));
	floor[51].rect.setPoint(3, sf::Vector2f(600, 680));

	floor[52].mu = 1;
	floor[52].direction = sf::Vector2f(1, 0);
	floor[52].rect.setPoint(0, sf::Vector2f(640, 640));
	floor[52].rect.setPoint(1, sf::Vector2f(680, 640));
	floor[52].rect.setPoint(2, sf::Vector2f(680, 680));
	floor[52].rect.setPoint(3, sf::Vector2f(640, 680));

	floor[53].mu = 1;
	floor[53].direction = sf::Vector2f(1, 0);
	floor[53].rect.setPoint(0, sf::Vector2f(680, 640));
	floor[53].rect.setPoint(1, sf::Vector2f(720, 640));
	floor[53].rect.setPoint(2, sf::Vector2f(720, 680));
	floor[53].rect.setPoint(3, sf::Vector2f(680, 680));

	floor[54].mu = 1;
	floor[54].direction = sf::Vector2f(1, 0);
	floor[54].rect.setPoint(0, sf::Vector2f(720, 640));
	floor[54].rect.setPoint(1, sf::Vector2f(760, 640));
	floor[54].rect.setPoint(2, sf::Vector2f(760, 680));
	floor[54].rect.setPoint(3, sf::Vector2f(720, 680));

	floor[55].mu = 1;
	floor[55].direction = sf::Vector2f(1, 0);
	floor[55].rect.setPoint(0, sf::Vector2f(760, 640));
	floor[55].rect.setPoint(1, sf::Vector2f(800, 640));
	floor[55].rect.setPoint(2, sf::Vector2f(800, 680));
	floor[55].rect.setPoint(3, sf::Vector2f(760, 680));

	floor[56].mu = 1;
	floor[56].direction = sf::Vector2f(1, 0);
	floor[56].rect.setPoint(0, sf::Vector2f(800, 640));
	floor[56].rect.setPoint(1, sf::Vector2f(840, 640));
	floor[56].rect.setPoint(2, sf::Vector2f(840, 680));
	floor[56].rect.setPoint(3, sf::Vector2f(800, 680));

	floor[57].mu = 1;
	floor[57].direction = sf::Vector2f(1, 0);
	floor[57].rect.setPoint(0, sf::Vector2f(840, 640));
	floor[57].rect.setPoint(1, sf::Vector2f(880, 640));
	floor[57].rect.setPoint(2, sf::Vector2f(880, 680));
	floor[57].rect.setPoint(3, sf::Vector2f(840, 680));

	//INITIALIZING PLAYERS
	
	Sphere ball1 = { {800, 100}, {0, 0}, {0, 0}, 20, 1, 255, 0 ,0 };
	
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
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (Qwer.play == true)
			{
				Qwer.play = false;
				ball1.speed = Qwer.direction(curpos) * Qwer.force(t);
			}
			else
			{
				Qwer.play = true;
			}
		}

		if (event.type == sf::Event::MouseMoved)
		{
			curpos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		}
		
		window.clear(sf::Color(0, 255, 0));
		
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
			t += DT;
		}
		else
		{
			Qwer.draw(&window);
		};

		drawShots(&window, sf::Vector2f(50, 400), sf::Vector2f(150, 400), sf::Vector2f(150, 500), sf::Vector2f(50, 500), 3);
		ball1.draw(&window);
		window.display();

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
