#pragma once
#include "math.h"
#include <SFML/Graphics.hpp>
#include <cassert>

struct Sphere
{
    sf::Vector2f pos;
    sf::Vector2f speed;
    sf::Vector2f acceleration;
    float r;
    float m;
    int red;
    int green;
    int blue;

    Sphere(sf::Vector2f pos = sf::Vector2f(0, 0), sf::Vector2f speed = sf::Vector2f(0, 0), sf::Vector2f acceleration = sf::Vector2f(0, 0), float r = 0, float m = 1, int red = 0, int green = 0, int blue = 0)
    {
        this->pos = pos;
        this->speed = speed;
        this->acceleration = acceleration;
        this->r = r;
        this->m = m;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    void draw(sf::RenderWindow* window, int lighting_detailing = 30)
    {
        float window_length = window->getSize().x;
        //std::cout << pos.x << "             " << pos.y;
        float window_width = window->getSize().y;
        assert((pos.x > r) && (pos.x + r < window_length) && (pos.y > r) && (pos.y + r < window_width));
        sf::CircleShape circle(r, 30);
        for (int i = 0; i < lighting_detailing; i++)
        {
            circle.setRadius(r - r * i / lighting_detailing);
            circle.setPosition(pos.x - r + 1.4 * r * i / lighting_detailing, pos.y - r + 0.6 * r * i / lighting_detailing);
            circle.setFillColor(sf::Color(0.875 * red * i / lighting_detailing + 0.125 * red, 0.875 * green * i / lighting_detailing + 0.125 * green, 0.875 * blue * i / lighting_detailing + 0.125 * blue));
            window->draw(circle);
        }
    }

    void move(const float DT)
    {
        speed = speed + acceleration * DT;
        pos = pos + speed * DT;
    }

    void collide(sf::ConvexShape* wall, const float DT)
    {
        sf::Vector2f newpos = pos + speed * DT;
        sf::Rect<float> testRect = { newpos - sf::Vector2f(r, r), sf::Vector2f(2 * r, 2 * r) };

        if (!testRect.intersects(wall->getGlobalBounds()))
        {}
        else if (wall->getGlobalBounds().contains(newpos))
        {
            std::cout << 2;
            if (abs(newpos.y - wall->getPoint(0).y) + abs(pos.y - wall->getPoint(0).y) == abs(newpos.y - pos.y) && wall->getPoint(0).x <= wall->getPoint(0).y * (newpos.x - pos.x) / (newpos.y - pos.y) + (newpos.y * pos.x - newpos.x * pos.y) / (newpos.y - pos.y) <= wall->getPoint(1).x)
            {
                pos.y = 2 * wall->getPoint(0).y - pos.y - 2 * r;
                speed.y *= -1;
            }
            else if (abs(newpos.x - wall->getPoint(1).x) + abs(pos.x - wall->getPoint(1).x) == abs(newpos.x - pos.x) && wall->getPoint(2).y <= wall->getPoint(1).x / (newpos.x - pos.x) * (newpos.y - pos.y) - (newpos.y * pos.x - newpos.x * pos.y) / (newpos.x - pos.x) <= wall->getPoint(1).y)
            {
                pos.x = 2 * wall->getPoint(1).x - pos.x + 2 * r;
                speed.x *= -1;
            }
            else if (abs(newpos.y - wall->getPoint(2).y) + abs(pos.y - wall->getPoint(2).y) == abs(newpos.y - pos.y) && wall->getPoint(3).x <= wall->getPoint(2).y * (newpos.x - pos.x) / (newpos.y - pos.y) + (newpos.y * pos.x - newpos.x * pos.y) / (newpos.y - pos.y) <= wall->getPoint(2).x)
            {
                pos.y = 2 * wall->getPoint(2).y - pos.y + 2 * r;
                speed.y *= -1;
            }
            else
            {
                pos.x = 2 * wall->getPoint(3).x - pos.x - 2 * r;
                speed.x *= -1;
            }
        }
        else if ((wall->getPoint(1).x >= newpos.x && newpos.x  >= wall->getPoint(0).x) || (wall->getPoint(1).y <= newpos.y && newpos.y <= wall->getPoint(2).y))
        {
            std::cout << 4;
            if (wall->getPoint(0).x <= newpos.x && newpos.x <= wall->getPoint(1).x)
            {
                if (newpos.y < wall->getPoint(0).y)
                {
                    pos.y = 2 * wall->getPoint(0).y - pos.y - 2 * r;
                    speed.y *= -1;
                }
                else
                {
                    pos.y = 2 * wall->getPoint(2).y - pos.y + 2 * r;
                    speed.y *= -1;
                }
            }
            else
            {
                if (newpos.x < wall->getPoint(0).x)
                {
                    pos.x = 2 * wall->getPoint(3).x - pos.x - 2 * r;
                    speed.x *= -1;
                }
                else
                {
                    pos.x = 2 * wall->getPoint(1).x - pos.x + 2 * r;
                    speed.x *= -1;
                }
            }
        }
        else if (pow(r, 2) > pow((newpos - wall->getPoint(0)).x, 2) + pow((newpos - wall->getPoint(0)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(1)).x, 2) + pow((newpos - wall->getPoint(1)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(2)).x, 2) + pow((newpos - wall->getPoint(2)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(3)).x, 2) + pow((newpos - wall->getPoint(3)).y, 2))
        {
            std::cout << "3   "; // Сравниваем время
            if (newpos.x < wall->getPoint(0).x)
            {
                if (newpos.y < wall->getPoint(0).y)
                {
                    float a = 1 + pow(speed.y / speed.x, 2);
                    float b = -2 * (newpos.x + speed.y / speed.x * (newpos.y - wall->getPoint(0).y) + wall->getPoint(0).x * pow(speed.y / speed.x, 2));
                    float c = pow(newpos.x, 2) + pow(wall->getPoint(0).y - newpos.y, 2) - 2 * (wall->getPoint(0).y - newpos.y) * speed.y / speed.x * wall->getPoint(0).x + pow(speed.y / speed.x * wall->getPoint(0).x, 2) - pow(r, 2);
                    float x = (-b + speed.x / abs(speed.x) * sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
                    float y = wall->getPoint(0).y + speed.y / speed.x * (x - wall->getPoint(0).x);
                    std::cout << "   x         " << x/* << "  y       " << y*/;
                    pos = wall->getPoint(0) - sf::Vector2f(x - wall->getPoint(0).x, y - wall->getPoint(0).y);
                    sf::Vector2f e = sf::Vector2f(x - newpos.x, y - newpos.y);
                    speed = e * (2 * (speed.x * e.x + speed.y * e.y) / ((x - newpos.x) * (x - newpos.x) + (y - newpos.y) * (y - newpos.y))) - speed;
                    //std::cout << speed.x << "              " << speed.y;
                    pos -= speed * DT;
                }
            }
        }

    }

    bool checkCollisionTwoSpheres(Sphere* sphere2)
    {
        return pow(pos.x - sphere2->pos.x, 2) + pow(pos.y - sphere2->pos.y, 2) < pow(r + sphere2->r, 2);
    }

    void collideSpheres(Sphere* sphere2, sf::RenderWindow* window)
    {
        float window_length = window->getSize().x;
        float window_width = window->getSize().y;
        float vx10 = speed.x;
        float vy10 = speed.y;
        float vx20 = sphere2->speed.x;
        float vy20 = sphere2->speed.y;
        float dist = sqrt(pow(sphere2->pos.x - pos.x, 2) + pow(sphere2->pos.y - pos.y, 2));
        float dist_x = sphere2->pos.x - pos.x;
        float dist_y = sphere2->pos.y - pos.y;
        assert(dist);

        if ((pos.x - (r + sphere2->r - dist) * dist_x / (2 * dist) > r) && (pos.x - (r + sphere2->r - dist) * dist_x / (2 * dist) < window_length - r) && (sphere2->pos.x + (r + sphere2->r - dist) * dist_x / (2 * dist) > sphere2->r) && (sphere2->pos.x + (r + sphere2->r - dist) * dist_x / (2 * dist) < window_length - sphere2->r))
        {
            pos.x -= (r + sphere2->r - dist) * dist_x / (2 * dist);
            sphere2->pos.x += (r + sphere2->r - dist) * dist_x / (2 * dist);
        }
        else if ((pos.x - (r + sphere2->r - dist) * dist_x / (2 * dist)) <= r)
            sphere2->pos.x += (r + sphere2->r - dist) * dist_x / dist;
        else if ((pos.x - (r + sphere2->r - dist) * dist_x / (2 * dist)) >= window_length - r)
            sphere2->pos.x += (r + sphere2->r - dist) * dist_x / dist;
        else if ((sphere2->pos.x + (r + sphere2->r - dist) * dist_x / (2 * dist)) <= sphere2->r)
            pos.x -= (r + sphere2->r - dist) * dist_x / dist;
        else
            pos.x -= (r + sphere2->r - dist) * dist_x / dist;

        if ((pos.y - (r + sphere2->r - dist) * dist_y / (2 * dist) > r) && (pos.y - (r + sphere2->r - dist) * dist_y / (2 * dist) < window_length - r) && (sphere2->pos.y + (r + sphere2->r - dist) * dist_y / (2 * dist) > sphere2->r) && (sphere2->pos.y + (r + sphere2->r - dist) * dist_y / (2 * dist) < window_length - sphere2->r))
        {
            pos.y -= (r + sphere2->r - dist) * dist_y / (2 * dist);
            sphere2->pos.y += (r + sphere2->r - dist) * dist_y / (2 * dist);
        }
        else if ((pos.y - (r + sphere2->r - dist) * dist_y / (2 * dist)) <= r)
            sphere2->pos.y += (r + sphere2->r - dist) * dist_y / dist;
        else if ((pos.y - (r + sphere2->r - dist) * dist_y / (2 * dist)) >= window_width - r)
            sphere2->pos.y += (r + sphere2->r - dist) * dist_y / dist;
        else if ((sphere2->pos.y + (r + sphere2->r - dist) * dist_y / (2 * dist)) <= sphere2->r)
            pos.y -= (r + sphere2->r - dist) * dist_y / dist;
        else
            pos.y -= (r + sphere2->r - dist) * dist_y / dist;

        speed.x = (2 * sphere2->m * vx20 + (m - sphere2->m) * vx10) / (m + sphere2->m);
        speed.y = (2 * sphere2->m * vy20 + (m - sphere2->m) * vy10) / (m + sphere2->m);
        sphere2->speed.x = (2 * m * vx10 + (sphere2->m - m) * vx20) / (m + sphere2->m);
        sphere2->speed.y = (2 * m * vy10 + (sphere2->m - m) * vy20) / (m + sphere2->m);
    }
};
