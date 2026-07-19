#pragma once
#include <SFML/Graphics.hpp>
#include "character.h"

class Player : public Character
{
public:
    Player(const sf::Texture &IdleTex, const sf::Texture &RunTex)
        : Character(IdleTex, RunTex, 200.f, {300.f, 400.f}, Team::player) {}

private:
    sf::Vector2f Get_direction() override
    {
        float x = 0.f, y = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            y += 1.f;
        return {x, y};
    }
};