#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "character.h"
#include "player.h"

class Bullet
{

private:
    float b_speed = 300.f;
    sf::Vector2f b_direction, b_position;
    const sf::Texture *b_Tex;
    sf::Sprite b_spr;
    bool b_alive = true;

public:
    Bullet(const sf::Texture &Tex, sf::Vector2f direction, sf::Vector2f position) : b_Tex(&Tex), b_spr(Tex), b_direction(direction), b_position(position)
    {
        b_spr.setTextureRect(sf::IntRect({0, 0}, {16, 16}));
        b_spr.setPosition(b_position);
        auto b = b_spr.getLocalBounds();
        b_spr.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }

    void update(float dt)
    {
        b_spr.move(b_direction * dt * b_speed);
    }

    bool check_distance() const
    {
        auto pos = b_spr.getPosition();
        float dx = pos.x - b_position.x;
        float dy = pos.y - b_position.y;
        return (dx * dx + dy * dy) > (150.f * 150.f);
    }

    void kill() { b_alive = false; }
    bool check_is_alive() { return b_alive; }
    void draw(sf::RenderWindow &w) const { w.draw(b_spr); }
    sf::Vector2f getPosition() const { return b_spr.getPosition(); }
};