#pragma once
#include "bullet.h"


class NormalB : public Bullet{
public:
    NormalB(const sf::Texture &Tex, sf::Vector2f direction, sf::Vector2f position)
        : Bullet(Tex, direction, position, 300.f, 10.f){}

    bool shouldEndFlight(const sf::FloatRect& visibleBounds) const override
    {
        const auto bounds = getBounds();
        // 整个精灵离开可见区域才回收，中心越界时仍可继续显示。
        return bounds.position.x + bounds.size.x <= visibleBounds.position.x ||
               bounds.position.y + bounds.size.y <= visibleBounds.position.y ||
               bounds.position.x >= visibleBounds.position.x + visibleBounds.size.x ||
               bounds.position.y >= visibleBounds.position.y + visibleBounds.size.y;
    }

};
