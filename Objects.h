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
        if ((pow(r, 2) > pow((newpos - wall->getPoint(0)).x, 2) + pow((newpos - wall->getPoint(0)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(1)).x, 2) + pow((newpos - wall->getPoint(1)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(2)).x, 2) + pow((newpos - wall->getPoint(2)).y, 2) || pow(r, 2) > pow((newpos - wall->getPoint(3)).x, 2) + pow((newpos - wall->getPoint(3)).y, 2)))
            return 2;
        return 0;
    }

    void collideWithWalls(sf::ConvexShape* walls, const float DT)
    {
        int collision_prop[5] = { 0, -1, -1 , -1, -1 };
        for (int i = 0; i < 11; i++)
        {
            int collision_type = this->checkCollisionWithWall(&walls[i], DT);
            if (collision_type)
            {
                collision_prop[2 * collision_prop[0] + 1] = collision_type;
                collision_prop[2 * collision_prop[0] + 2] = i;
                collision_prop[0]++;
            }
        }
        if (collision_prop[0] == 1)
        {
            this->physicalPartOfCollidingWithWalls(&walls[collision_prop[2]], DT, collision_prop[1]);
        }
        if (collision_prop[0] == 2)
        {
            if (collision_prop[1] == 2 && collision_prop[3] == 2)
            {
                this->physicalPartOfCollidingWithWalls(&walls[collision_prop[2]], DT, 2);
            }
            else if (collision_prop[1] == 1 && collision_prop[3] == 1)
            {
                this->physicalPartOfCollidingWithWalls(&walls[collision_prop[2]], DT, 1);
                if (this->checkCollisionWithWall(&walls[collision_prop[4]], DT))
                {
                    this->physicalPartOfCollidingWithWalls(&walls[collision_prop[4]], DT, 1);
                }
            }
            else if (collision_prop[1] == 1)
            {
                this->physicalPartOfCollidingWithWalls(&walls[collision_prop[2]], DT, 1);
            }
            else
            {
                this->physicalPartOfCollidingWithWalls(&walls[collision_prop[4]], DT, 1);
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
