#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <vector>
#include "enemy.h"

class Bullet
{
protected:
    float damage = 10.f;
    float b_speed = 300.f;


private:
    sf::Vector2f b_direction, b_position;
    const sf::Texture *b_Tex;
    sf::Sprite b_spr;
    bool b_alive = true;
    float b_distance = 0.f;
    static constexpr float max_distance = 200.f;

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

    virtual void onRangeEnd(const std::vector<std::unique_ptr<Enemy>>&)
    {
        kill();
    }

    void update(float dt)
    {
        if (!check_is_alive()) return;
        const float remaining = max_distance - b_distance;
        const float step = dt * b_speed;
        const float distance = step < remaining ? step : remaining;
        b_spr.move(b_direction * distance);
        b_distance = step >= remaining ? max_distance : b_distance + distance;
    }

    bool check_distance() const
    {
        return b_distance >= max_distance;
    }

    float getDamage() const {return damage;}
    void kill() { b_alive = false; }
    bool check_is_alive() const { return b_alive; }
    void draw(sf::RenderWindow &w) const { w.draw(b_spr); }
    sf::Vector2f getPosition() const { return b_spr.getPosition(); }
};
