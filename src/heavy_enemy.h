#pragma once
#include "enemy.h"

class HeavyE : public Enemy
{
public:
    HeavyE(const sf::Texture& idleTexture, const sf::Texture& runTexture,
           const Player* target, sf::Vector2f position)
        : Enemy(idleTexture, runTexture, target, position, 100.f, 300.f) {}
};
