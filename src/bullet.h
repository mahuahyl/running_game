#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <vector>
#include <algorithm>
#include <limits>
#include "enemy.h"

class Bullet
{
protected:
    float damage = 10.f;
    float b_speed = 300.f;

    // 默认不限射程。子类可以限制单次移动，保证终点不会被跨过。
    virtual float getFlightLimit() const { return std::numeric_limits<float>::infinity(); }
    float getDistanceTraveled() const { return b_distance; }
    sf::FloatRect getBounds() const { return b_spr.getGlobalBounds(); }


private:
    sf::Vector2f b_direction, b_position;
    const sf::Texture *b_Tex;
    sf::Sprite b_spr;
    bool b_alive = true;
    float b_distance = 0.f;

public:
    Bullet(const sf::Texture &Tex, sf::Vector2f direction, sf::Vector2f position, float speed, float damageValue)
         : damage(damageValue), b_speed(speed), b_direction(direction), b_position(position), b_Tex(&Tex), b_spr(Tex)
    {
        const float len = std::hypot(b_direction.x, b_direction.y);
        if (len > 0.f)
            b_direction /= len;
        else
            kill();
        b_spr.setTextureRect(sf::IntRect({0, 0}, {16, 16}));
        b_spr.setPosition(b_position);
        auto b = b_spr.getLocalBounds();
        b_spr.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }

    virtual ~Bullet() = default;

    // 普通弹没有范围效果；爆炸弹提供与伤害判定相同的半径。
    virtual float getExplosionRadius() const { return 0.f; }

    virtual void onImpact(Enemy& target, const std::vector<std::unique_ptr<Enemy>>&)
    {
        if (!check_is_alive()) return;
        target.hitted(getDamage());
        kill();
    }

    virtual void onFlightEnd(const std::vector<std::unique_ptr<Enemy>>&)
    {
        kill();
    }

    void update(float dt)
    {
        if (!check_is_alive()) return;
        const float limit = getFlightLimit();
        const float remaining = std::max(0.f, limit - b_distance);
        const float step = dt * b_speed;
        const float distance = step < remaining ? step : remaining;
        b_spr.move(b_direction * distance);
        b_distance = step >= remaining ? limit : b_distance + distance;
    }

    // Game 提供可见区域，各子类决定自己的飞行结束条件。
    virtual bool shouldEndFlight(const sf::FloatRect& visibleBounds) const = 0;

    float getDamage() const {return damage;}
    void kill() { b_alive = false; }
    bool check_is_alive() const { return b_alive; }
    void draw(sf::RenderWindow &w) const { w.draw(b_spr); }
    sf::Vector2f getPosition() const { return b_spr.getPosition(); }
};
