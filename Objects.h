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
    bool flag;

    Sphere(sf::Vector2f pos = sf::Vector2f(0, 0), sf::Vector2f speed = sf::Vector2f(0, 0), sf::Vector2f acceleration = sf::Vector2f(0, 0), float r = 0, float m = 1, int red = 0, int green = 0, int blue = 0, bool flag = false)
    {
        this->pos = pos;
        this->speed = speed;
        this->acceleration = acceleration;
        this->r = r;
        this->m = m;
        this->red = red;
        this->green = green;
        this->blue = blue;
        this->flag = flag;
        
    }

    void draw(sf::RenderWindow* window, int lighting_detailing = 30)
    {
        float window_length = window->getSize().x;
        float window_width = window->getSize().y;
        sf::CircleShape circle1(r, 30);
        sf::CircleShape circle2(r * 2 / 3, 30);
        circle1.setPosition(pos.x - r, pos.y - r);
        circle1.setFillColor(sf::Color(255, 0, 0));
        circle1.setOutlineColor(sf::Color(0, 0, 0));
        circle1.setOutlineThickness(2);
        circle2.setPosition(pos.x - r * 2 / 3, pos.y - r * 2 / 3);
        circle2.setFillColor(sf::Color(255, 128, 128));
        window->draw(circle1);
        window->draw(circle2);
    }

    void move(const float DT)
    {
        speed = speed + acceleration * DT;
        if (pow(speed.x, 2) + pow(speed.y, 2) > 225)
            speed = speed / float(pow(speed.x, 2) + pow(speed.y, 2)) * 15.f;
        pos = pos + speed * DT;
    }

    void friction(Floor* floor, int len)
    {
        for (int i = 0; i < len; i++)
        {
            if (floor[i].rect.getGlobalBounds().contains(pos))
            {
                if (floor[i].mu < 0)
                {
                    acceleration = speed / float(sqrt(pow(speed.x, 2) + pow(speed.y, 2))) / (-10.f);
                    if (pow(speed.x, 2) + pow(speed.y, 2) < 0.1)
                        acceleration = speed * (-1.f);
                    break;
                }
                else
                {
                    acceleration = floor[i].direction;
                    break;
                }
            }
            else if (i == len - 1)
            {
                acceleration = speed / float(sqrt(pow(speed.x, 2) + pow(speed.y, 2))) / (-50.f);
                if (pow(speed.x, 2) + pow(speed.y, 2) < 0.02)
                    acceleration = speed * (-1.f);
            }
        }
    }

    void collide(sf::ConvexShape* wall, const float DT)
    {
        sf::Vector2f newpos = pos + speed * DT;
        sf::Rect<float> testRect = { newpos - sf::Vector2f(r, r), sf::Vector2f(2 * r, 2 * r) };

        if (!testRect.intersects(wall->getGlobalBounds()))
        {}
        else if (wall->getGlobalBounds().contains(newpos))
        {
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
        else if ((!flag) && (pow(r, 2) > pow((newpos - wall->getPoint(0)).x, 2) + pow((newpos - wall->getPoint(0)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(1)).x, 2) + pow((newpos - wall->getPoint(1)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(2)).x, 2) + pow((newpos - wall->getPoint(2)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(3)).x, 2) + pow((newpos - wall->getPoint(3)).y, 2)))
        {
            if (newpos.x < wall->getPoint(0).x)
            {
                if (newpos.y < wall->getPoint(0).y)
                {
                    float x;
                    float y;
                    if (speed.x == 0)
                    {
                        x = wall->getPoint(0).x;
                        y = newpos.y + sqrt(pow(r, 2) - pow(x - newpos.x, 2)) * speed.y / abs(speed.y);
                    }
                    else
                    {
                        float a = 1 + pow(speed.y / speed.x, 2);
                        float b = -2 * (newpos.x + speed.y / speed.x * (newpos.y - wall->getPoint(0).y) + wall->getPoint(0).x * pow(speed.y / speed.x, 2));
                        float c = pow(newpos.x, 2) + pow(wall->getPoint(0).y - newpos.y, 2) - 2 * (wall->getPoint(0).y - newpos.y) * speed.y / speed.x * wall->getPoint(0).x + pow(speed.y / speed.x * wall->getPoint(0).x, 2) - pow(r, 2);
                        x = (-b + speed.x / abs(speed.x) * sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
                        y = wall->getPoint(0).y + speed.y / speed.x * (x - wall->getPoint(0).x);
                    }
                    pos = newpos - sf::Vector2f(x - wall->getPoint(0).x, y - wall->getPoint(0).y);
                    sf::Vector2f e = sf::Vector2f(x - newpos.x, y - newpos.y);
                    speed = e * (-2 * (speed.x * e.x + speed.y * e.y) / ((x - newpos.x) * (x - newpos.x) + (y - newpos.y) * (y - newpos.y))) + speed;
                    pos -= speed * DT;
                    flag = true;
                }
                else
                {
                    float x;
                    float y;
                    if (speed.x == 0)
                    {
                        x = wall->getPoint(3).x;
                        y = newpos.y + sqrt(pow(r, 2) - pow(x - newpos.x, 2)) * speed.y / abs(speed.y);
                    }
                    else
                    {
                        float a = 1 + pow(speed.y / speed.x, 2);
                        float b = -2 * (newpos.x + speed.y / speed.x * (newpos.y - wall->getPoint(3).y) + wall->getPoint(3).x * pow(speed.y / speed.x, 2));
                        float c = pow(newpos.x, 2) + pow(wall->getPoint(3).y - newpos.y, 2) - 2 * (wall->getPoint(3).y - newpos.y) * speed.y / speed.x * wall->getPoint(3).x + pow(speed.y / speed.x * wall->getPoint(3).x, 2) - pow(r, 2);
                        x = (-b + speed.x / abs(speed.x) * sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
                        y = wall->getPoint(3).y + speed.y / speed.x * (x - wall->getPoint(3).x);
                    }
                    pos = newpos - sf::Vector2f(x - wall->getPoint(3).x, y - wall->getPoint(3).y);
                    sf::Vector2f e = sf::Vector2f(x - newpos.x, y - newpos.y);
                    speed = e * (-2 * (speed.x * e.x + speed.y * e.y) / ((x - newpos.x) * (x - newpos.x) + (y - newpos.y) * (y - newpos.y))) + speed;
                    pos -= speed * DT;
                    flag = true;
                }
            }
            else
            {
                if (newpos.y < wall->getPoint(1).y)
                {
                    float x;
                    float y;
                    if (speed.x == 0)
                    {
                        x = wall->getPoint(1).x;
                        y = newpos.y + sqrt(pow(r, 2) - pow(x - newpos.x, 2)) * speed.y / abs(speed.y);
                    }
                    else
                    {
                        float a = 1 + pow(speed.y / speed.x, 2);
                        float b = -2 * (newpos.x + speed.y / speed.x * (newpos.y - wall->getPoint(1).y) + wall->getPoint(1).x * pow(speed.y / speed.x, 2));
                        float c = pow(newpos.x, 2) + pow(wall->getPoint(1).y - newpos.y, 2) - 2 * (wall->getPoint(1).y - newpos.y) * speed.y / speed.x * wall->getPoint(1).x + pow(speed.y / speed.x * wall->getPoint(1).x, 2) - pow(r, 2);
                        x = (-b + speed.x / abs(speed.x) * sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
                        y = wall->getPoint(1).y + speed.y / speed.x * (x - wall->getPoint(1).x);
                    }
                    pos = newpos - sf::Vector2f(x - wall->getPoint(1).x, y - wall->getPoint(1).y);
                    sf::Vector2f e = sf::Vector2f(x - newpos.x, y - newpos.y);
                    speed = e * (-2 * (speed.x * e.x + speed.y * e.y) / ((x - newpos.x) * (x - newpos.x) + (y - newpos.y) * (y - newpos.y))) + speed;
                    pos -= speed * DT;
                    flag = true;
                }
                else
                {
                    float x;
                    float y;
                    if (speed.x == 0)
                    {
                        x = wall->getPoint(2).x;
                        y = newpos.y + sqrt(pow(r, 2) - pow(x - newpos.x, 2)) * speed.y / abs(speed.y);
                    }
                    else
                    {
                        float a = 1 + pow(speed.y / speed.x, 2);
                        float b = -2 * (newpos.x + speed.y / speed.x * (newpos.y - wall->getPoint(2).y) + wall->getPoint(2).x * pow(speed.y / speed.x, 2));
                        float c = pow(newpos.x, 2) + pow(wall->getPoint(2).y - newpos.y, 2) - 2 * (wall->getPoint(2).y - newpos.y) * speed.y / speed.x * wall->getPoint(2).x + pow(speed.y / speed.x * wall->getPoint(2).x, 2) - pow(r, 2);
                        x = (-b + speed.x / abs(speed.x) * sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
                        y = wall->getPoint(2).y + speed.y / speed.x * (x - wall->getPoint(2).x);
                    }
                    pos = newpos - sf::Vector2f(x - wall->getPoint(2).x, y - wall->getPoint(2).y);
                    sf::Vector2f e = sf::Vector2f(x - newpos.x, y - newpos.y);
                    speed = e * (-2 * (speed.x * e.x + speed.y * e.y) / ((x - newpos.x) * (x - newpos.x) + (y - newpos.y) * (y - newpos.y))) + speed;
                    pos -= speed * DT;
                    flag = true;
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
