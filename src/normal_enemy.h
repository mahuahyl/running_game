#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "player.h"
#include "character.h"
#include "enemy.h"

class NormalE : public Enemy{
public:
    NormalE(const sf::Texture &IdleTex, const sf::Texture &RunTex, const Player *target, sf::Vector2f position)
        : Enemy(IdleTex, RunTex, target, position, 150.f, 50.f) {}   
};