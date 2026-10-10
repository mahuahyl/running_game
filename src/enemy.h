#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "player.h"
#include "character.h"

class Enemy : public Character
{
private:
    const Player *m_target;

public:


    Enemy(const sf::Texture &IdleTex, const sf::Texture &RunTex, const Player *target, sf::Vector2f position, float speed, float Hp)
        : Character(IdleTex, RunTex, position, Team::Enemy, speed, Hp), m_target(target) {}

    bool check_if_dead() const {
        return (m_Hp <= 0.f);
    }

private:
    sf::Vector2f getDirection() override
    {
        sf::Vector2f aim_postion = m_target->getPosition(), m_position = this->getPosition();
        sf::Vector2f direction = {aim_postion.x - m_position.x, aim_postion.y - m_position.y};

        if (sqrt(direction.x * direction.x + direction.y * direction.y) <= 10.f)
            return {0.f, 0.f};

        return direction;
    }
};