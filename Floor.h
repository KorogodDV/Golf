#pragma once
#include "math.h"
#include <SFML/Graphics.hpp>
#include <cassert>

struct Floor
{
    sf::ConvexShape rect;
    float mu;
    sf::Vector2f direction;

    Floor(sf::ConvexShape shape = sf::ConvexShape(4), float mu = 0, sf::Vector2f direction = sf::Vector2f(0, 0))
    {
        rect = shape;
        this->mu = mu;
        this->direction = direction;
    }

    Floor(float mu, sf::Vector2f direction, sf::Vector2f A, sf::Vector2f B, sf::Vector2f C, sf::Vector2f D)
    {
        this->mu = mu;
        this->direction = direction;
        rect = sf::ConvexShape(4);
        rect.setPoint(0, A);
        rect.setPoint(1, B);
        rect.setPoint(2, C);
        rect.setPoint(3, D);
    }

    void draw(sf::RenderWindow* window)
    {
        if (mu < 0)
        {
            rect.setFillColor(sf::Color(255, 255, 0));
            window->draw(rect);
        }

        if (mu > 0)
        {
            rect.setFillColor(sf::Color(0, 128, 128));
            rect.setOutlineColor(sf::Color(0, 0, 0));
            rect.setOutlineThickness(2);

            sf::Vector2f A = rect.getPoint(0);
            sf::Vector2f B = rect.getPoint(1);
            sf::Vector2f C = rect.getPoint(2);
            sf::Vector2f D = rect.getPoint(3);

            float length = B.x - A.x;
            sf::Vector2f Center(A.x + length / 2, A.y + length / 2);
            float lengthArrow = length * 3 / 8;
            float widthArrow = lengthArrow / 3;
            sf::Vector2f pdirection(direction.y, direction.x);
            sf::Vector2f A1 = Center - direction * lengthArrow + pdirection * widthArrow;
            sf::Vector2f B1 = Center + direction * (lengthArrow / 2) + pdirection * widthArrow;
            sf::Vector2f C1 = Center + direction * (lengthArrow / 2) - pdirection * widthArrow;
            sf::Vector2f D1 = Center - direction * lengthArrow - pdirection * widthArrow;
            sf::Vector2f E1 = B1 + pdirection * widthArrow;
            sf::Vector2f F1 = C1 - pdirection * widthArrow;
            sf::Vector2f G1 = Center + direction * lengthArrow;

            sf::ConvexShape arrow1;
            sf::ConvexShape arrow2;
            arrow1.setPointCount(4);
            arrow2.setPointCount(3);
            arrow1.setPoint(0, A1);
            arrow1.setPoint(1, B1);
            arrow1.setPoint(2, C1);
            arrow1.setPoint(3, D1);

            arrow2.setPoint(0, E1);
            arrow2.setPoint(1, G1);
            arrow2.setPoint(2, F1);

            arrow1.setFillColor(sf::Color(255, 0, 0));
            arrow2.setFillColor(sf::Color(255, 0, 0));

            window->draw(rect);
            window->draw(arrow1);
            window->draw(arrow2);
        }
    }
};
