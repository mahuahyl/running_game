#pragma once
#include <SFML/Graphics.hpp>
#include "character.h"
#include "player.h"

class Follower : public Character{
private:
    const Player* m_target;
    


public:
    Follower(const sf::Texture &IdleTex, const sf::Texture &RunTex, const Player* target)
    : Character(IdleTex, RunTex, 100.f), m_target(target){}

private:
    sf::Vector2f  Get_direction() override{
        sf::Vector2f aim_postion = m_target->getPosition(), m_position = this->getPosition();
        sf::Vector2f direction = {aim_postion.x - m_position.x, aim_postion.y - m_position.y};

        if(sqrt(direction.x * direction.x + direction.y * direction.y) <= 10.f) return {0.f, 0.f};
        
        return direction;
    }

};