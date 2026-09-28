#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "player.h"
#include "character.h"
#include "enemy.h"

class FastE : public Enemy{
public:
    FastE(const sf::Texture &IdleTex, const sf::Texture &RunTex, const Player *target, sf::Vector2f position)
        : Enemy(IdleTex, RunTex, target, position, 250.f, 10.f) {}   
};