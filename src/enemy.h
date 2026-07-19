#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "player.h"
#include "character.h"

class Enemy : public Character
{
private:
    const Player *m_target;
    float m_Hp = 100.f;

public:


    Enemy(const sf::Texture &IdleTex, const sf::Texture &RunTex, const Player *target)
        : Character(IdleTex, RunTex, 150.f, {100.f, 200.f}, Team::Enemy), m_target(target) {}

    bool check_if_dead(){
        return (m_Hp <= 0.f);
    }

    void Heated(){
        this->m_Hp -= 5.f;
    }

private:
    sf::Vector2f Get_direction() override
    {
        sf::Vector2f aim_postion = m_target->getPosition(), m_position = this->getPosition();
        sf::Vector2f direction = {aim_postion.x - m_position.x, aim_postion.y - m_position.y};

        if (sqrt(direction.x * direction.x + direction.y * direction.y) <= 10.f)
            return {0.f, 0.f};

        return direction;
    }
};