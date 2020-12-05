#pragma once
#include "math.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "Sphere.h"

struct Player
{
    Sphere ball;
    int count;
    sf::Vector2f pos;
    float radius;
    int red;
    int green;
    int blue;
    sf::Vector2f posShots;
    float lengthShots;
    bool play;

    Player(Sphere ball, int count, sf::Vector2f pos, float radius, int red, int green, int blue, sf::Vector2f posShots, float lengthShots, bool play)
    {
        this->ball = ball;
        this->count = count;
        this->radius = radius;
        this->pos = pos;
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->posShots = posShots;
        this->lengthShots = lengthShots;
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

    void draw(sf::RenderWindow* window, sf::Vector2f posCursor, float t, sf::Font* font)
    {
        sf::CircleShape circle1(0, 30);
        circle1.setRadius(radius);
        circle1.setFillColor(sf::Color(red, green, blue));
        circle1.setPosition(pos - (sf::Vector2f(1, 1) * radius));
        sf::CircleShape circle2(0, 30);
        circle2.setRadius(radius / 4);
        circle2.setFillColor(sf::Color(red / 2, green / 2, blue / 2));
        circle2.setPosition(pos - (sf::Vector2f(1, 1) * radius / float(4)));

        sf::Vector2f A1 = posShots + sf::Vector2f(-1, -1) * lengthShots / 2.f;
        sf::Vector2f D1 = posShots + sf::Vector2f(-1, 1) * lengthShots / 2.f;
        sf::Vector2f B1 = posShots + sf::Vector2f(1, -1) * lengthShots / 2.f;
        sf::Vector2f C1 = posShots + sf::Vector2f(1, 1) * lengthShots / 2.f;

        sf::ConvexShape shape;
        shape.setPointCount(4);
        shape.setPoint(0, A1);
        shape.setPoint(1, B1);
        shape.setPoint(2, C1);
        shape.setPoint(3, D1);
        shape.setFillColor(sf::Color(0, 0, 0));

        sf::Text text("S H O T S", *font, 15);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition(A1 + (D1 - A1) / 4.f + (B1 - A1) / 10.f);

        std::string str = std::to_string(count);
        sf::Text text1(str, *font, 30);
        text1.setFillColor(sf::Color(red, green, blue));
        text1.setPosition(A1 + (D1 - A1) * 0.6f + (B1 - A1) * 0.4f);

        window->draw(shape);
        window->draw(text);
        window->draw(text1);
        window->draw(circle1);
        window->draw(circle2);
        ball.draw(window);

        if (play)
        {
            sf::Vector2f dir = direction(posCursor);
            float f = force(t) * 20;

            sf::Vector2f ort(-dir.y, dir.x);
            float length = sqrt(pow(ort.x, 2) + pow(ort.y, 2));
            ort = ort / length;

            sf::Vector2f A = ball.pos + ort * 3.f;
            sf::Vector2f D = ball.pos - ort * 3.f;
            sf::Vector2f B = ball.pos + dir * 100.f + ort * 3.f;
            sf::Vector2f C = ball.pos + dir * 100.f - ort * 3.f;
            sf::Vector2f E = B + ort * 3.f;
            sf::Vector2f F = C - ort * 3.f;
            sf::Vector2f G = ball.pos + dir * 120.f;
            sf::Vector2f B2 = ball.pos + dir * f + ort * 3.f;
            sf::Vector2f C2 = ball.pos + dir * f - ort * 3.f;

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
            arrow3.setPoint(1, B2);
            arrow3.setPoint(2, C2);
            arrow3.setPoint(3, D);

            arrow1.setFillColor(sf::Color(14, 241, 255));
            arrow2.setFillColor(sf::Color(14, 241, 255));
            arrow3.setFillColor(sf::Color(0, 0, 255));

            window->draw(arrow1);
            window->draw(arrow2);
            window->draw(arrow3);
        }
    }
};