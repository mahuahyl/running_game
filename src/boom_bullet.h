#pragma once
#include "bullet.h"

class BoomB : public Bullet{
private:
    float bound = 50.f;

    void explode(const std::vector<std::unique_ptr<Enemy>>& enemies)
    {
        if (!check_is_alive()) return;
        // 先结束飞行，保证命中与射程结束只触发一次伤害。
        kill();
        const auto center = getPosition();
        for (const auto& enemy : enemies)
        {
            if (enemy->check_if_dead()) continue;
            const auto offset = enemy->getPosition() - center;
            if (offset.x * offset.x + offset.y * offset.y <= bound * bound)
                enemy->hitted(getDamage());
        }
    }

public:
    BoomB(const sf::Texture &Tex, sf::Vector2f direction, sf::Vector2f position)
        : Bullet(Tex, direction, position, 150.f, 100.f){}

    float getExplosionRadius() const override { return bound; }

    void onImpact(Enemy&, const std::vector<std::unique_ptr<Enemy>>& enemies) override
    {
        explode(enemies);
    }

    void onRangeEnd(const std::vector<std::unique_ptr<Enemy>>& enemies) override
    {
        explode(enemies);
    }


};
