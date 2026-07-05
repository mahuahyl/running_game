#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <windows.h>
#include <cmath>

class Player
{
private:
    sf::Sprite spr;
    float speed = 200.f;
    int frame = 0;
    sf::Clock animClock;

public:
    Player(const sf::Texture &tex) : spr(tex)
    {
        spr.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
        auto b = spr.getLocalBounds();
        spr.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }

    void move(float dt)
    {
        double angle = 0;
        float x = 0, y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            x += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            x -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            y -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            y += 1.f;
        }
        if (animClock.getElapsedTime().asSeconds() > 0.1f)
        {
            frame = (frame + 1) % 12;
            spr.setTextureRect(sf::IntRect({frame * 32, 0}, {32, 32}));
            animClock.restart();
        }
        float len = std::sqrt(x * x + y * y);
        if (len > 0.f)
        {
            x /= len;
            y /= len;
        }

        spr.move({speed * x * dt, speed * y * dt});
    }

    void draw(sf::RenderWindow &w) const { w.draw(spr); }
};