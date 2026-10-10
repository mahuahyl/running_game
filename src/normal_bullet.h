#pragma once
#include "bullet.h"


class NormalB : public Bullet{
public:
    NormalB(const sf::Texture &Tex, sf::Vector2f direction, sf::Vector2f position)
        : Bullet(Tex, direction, position, 300.f, 10.f){}


};