#pragma once
#include "math.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "Floor.h"

float len(sf::Vector2f vect)
{
    return sqrt(pow(vect.x, 2) + pow(vect.y, 2));
}

sf::Vector2f calculatingIntersectionPoint(sf::Vector2f speed, sf::Vector2f point, sf::Vector2f newpos, float r)
{
    sf::Vector2f res;
    if (speed.x == 0)
    {
        res.x = point.x;
        res.y = newpos.y + sqrt(pow(r, 2) - pow(res.x - newpos.x, 2)) * speed.y / abs(speed.y);
        return res;
    }
    float a = 1 + pow(speed.y / speed.x, 2);
    float b = -2 * (newpos.x + speed.y / speed.x * (newpos.y - point.y) + point.x * pow(speed.y / speed.x, 2));
    float c = pow(newpos.x, 2) + pow(point.y - newpos.y, 2) - 2 * (point.y - newpos.y) * speed.y / speed.x * point.x + pow(speed.y / speed.x * point.x, 2) - pow(r, 2);
    res.x = (-b + speed.x / abs(speed.x) * sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    res.y = point.y + speed.y / speed.x * (res.x - point.x);
    return res;
}

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

    void draw(sf::RenderWindow* window, int lighting_detailing = 2)
    {
        float window_length = window->getSize().x;
        float window_width = window->getSize().y;
        sf::CircleShape circle1(r, 30);
        sf::CircleShape circle2(r * 2 / 3, 30);
        circle1.setPosition(pos.x - r, pos.y - r);
        circle1.setFillColor(sf::Color(red, green, blue));
        circle1.setOutlineColor(sf::Color(0, 0, 0));
        circle1.setOutlineThickness(2);
        circle2.setPosition(pos.x - r * 2 / 3, pos.y - r * 2 / 3);
        circle2.setFillColor(sf::Color(red, green + 128, blue + 128)); 
        window->draw(circle1);
        window->draw(circle2);
    }

    void move(const float DT)
    {
        speed = speed + acceleration * DT;
        acceleration = sf::Vector2f(0, 0);
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
                    acceleration = floor[i].direction / 10.f;
                    break;
                }
            }
            else if (i == len - 1)
            {
                acceleration = speed / float(sqrt(pow(speed.x, 2) + pow(speed.y, 2))) / (-50.f);
                if (pow(speed.x, 2) + pow(speed.y, 2) > 25)
                    acceleration = speed * float(-0.15);
                if (pow(speed.x, 2) + pow(speed.y, 2) < 0.02)
                    acceleration = speed * (-1.f);
            }
        }
    }

    int checkCollisionWithWall(sf::ConvexShape* wall, const float DT)
    {
        sf::Vector2f newpos = pos + speed * DT;
        sf::Rect<float> testRect = { newpos - sf::Vector2f(r, r), sf::Vector2f(2 * r, 2 * r) };

        if (!testRect.intersects(wall->getGlobalBounds()))
            return 0;
        if ((wall->getPoint(1).x >= newpos.x && newpos.x >= wall->getPoint(0).x) || (wall->getPoint(1).y <= newpos.y && newpos.y <= wall->getPoint(2).y))
            return 1;
        if (len(newpos - wall->getPoint(0)) < r || len(newpos - wall->getPoint(1)) < r || len(newpos - wall->getPoint(2)) < r || len(newpos - wall->getPoint(3)) < r)
            return 2;
        return 0;
    }

    void collideWithWalls(sf::ConvexShape* walls, const float DT)
    {
        int collisions[5] = { 0, -1, -1 , -1, -1 };
        for (int i = 0; i < 11; i++)
        {
            int collision_type = this->checkCollisionWithWall(&walls[i], DT);
            if (collision_type)
            {
                collisions[2 * collisions[0] + 1] = collision_type;
                collisions[2 * collisions[0] + 2] = i;
                collisions[0]++;
            }
        }
        if (collisions[0] == 1)
        {
            this->physicalPartOfCollidingWithWalls(&walls[collisions[2]], DT, collisions[1]);
        }
        if (collisions[0] == 2)
        {
            if (collisions[1] == 2 && collisions[3] == 2)
            {
                this->physicalPartOfCollidingWithWalls(&walls[collisions[2]], DT, 2);
            }
            else if (collisions[1] == 1 && collisions[3] == 1)
            {
                this->physicalPartOfCollidingWithWalls(&walls[collisions[2]], DT, 1);
                if (this->checkCollisionWithWall(&walls[collisions[4]], DT))
                {
                    this->physicalPartOfCollidingWithWalls(&walls[collisions[4]], DT, 1);
                }
            }
            else if (collisions[1] == 1)
            {
                this->physicalPartOfCollidingWithWalls(&walls[collisions[2]], DT, 1);
            }
            else
            {
                this->physicalPartOfCollidingWithWalls(&walls[collisions[4]], DT, 1);
            }
        }
    }

    void physicalPartOfCollidingWithWalls(sf::ConvexShape* wall, const float DT, int type)
    {
        sf::Vector2f newpos = pos + speed * DT;

        if (type == 0)
            return;
        if (type == 1)
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
        else
        {
            sf::Vector2f intersectionPoint;
            if (newpos.x < wall->getPoint(0).x)
            {
                if (newpos.y < wall->getPoint(0).y)
                {
                    intersectionPoint = calculatingIntersectionPoint(speed, wall->getPoint(0), newpos, r);
                    pos = newpos - intersectionPoint + wall->getPoint(0);
                }
                else
                {
                    intersectionPoint = calculatingIntersectionPoint(speed, wall->getPoint(3), newpos, r);
                    pos = newpos - intersectionPoint + wall->getPoint(3);
                }
            }
            else
            {
                if (newpos.y < wall->getPoint(1).y)
                {
                    intersectionPoint = calculatingIntersectionPoint(speed, wall->getPoint(1), newpos, r);
                    pos = newpos - intersectionPoint + wall->getPoint(1);
                }
                else
                {
                    intersectionPoint = calculatingIntersectionPoint(speed, wall->getPoint(2), newpos, r);
                    pos = newpos - intersectionPoint + wall->getPoint(2);
                }
            }
            speed = (intersectionPoint - newpos) * (-2 * (speed.x * (intersectionPoint - newpos).x + speed.y * (intersectionPoint - newpos).y) / float(pow(len((intersectionPoint - newpos)), 2))) + speed;
            pos -= speed * DT;
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

        sf::Vector2f dspeed = (sphere2->pos - pos) * ((sphere2->speed.x - speed.x) * (sphere2->pos - pos).x + (sphere2->speed.y - speed.y) * (sphere2->pos - pos).y) / (4 * r * r);
        speed = speed + dspeed;
        sphere2->speed = sphere2->speed - dspeed;
    }
};
