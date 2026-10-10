#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint>

// 只负责显示。伤害在触发爆炸时已由 BoomB 一次性结算。
class Explosion
{
private:
    sf::Vector2f center;
    float radius;
    float elapsed = 0.f;
    static constexpr float duration = 0.5f;
    static constexpr float pi = 3.14159265f;

public:
    Explosion(sf::Vector2f position, float blastRadius)
        : center(position), radius(blastRadius) {}

    void update(float dt) { elapsed += dt; }
    bool isFinished() const { return elapsed >= duration; }

    void draw(sf::RenderWindow& window) const
    {
        if (isFinished()) return;
        const float progress = std::clamp(elapsed / duration, 0.f, 1.f);
        const auto alpha = static_cast<std::uint8_t>(255.f * (1.f - progress));

        // 边界从第一帧起就是实际伤害半径；轮廓向内画，不扩大范围。
        sf::CircleShape boundary(radius, 96);
        boundary.setOrigin({radius, radius});
        boundary.setPosition(center);
        boundary.setFillColor(sf::Color(255, 140, 40, static_cast<std::uint8_t>(alpha / 8)));
        boundary.setOutlineColor(sf::Color(255, 195, 85, alpha));
        boundary.setOutlineThickness(-2.f);
        window.draw(boundary);

        // 冲击波扩散至边界，在后半段淡出。
        const float expansion = std::min(progress / 0.65f, 1.f);
        const float waveRadius = radius * (0.08f + 0.92f * expansion);
        sf::CircleShape wave(waveRadius, 32);
        wave.setOrigin({waveRadius, waveRadius});
        wave.setPosition(center);
        wave.setFillColor(sf::Color::Transparent);
        wave.setOutlineColor(sf::Color(255, 110, 35, alpha));
        wave.setOutlineThickness(-4.f);
        window.draw(wave);

        // 低边数火光和方形火星与现有像素素材搭配。
        const float coreRadius = radius * (0.12f + 0.22f * expansion);
        sf::CircleShape core(coreRadius, 12);
        core.setOrigin({coreRadius, coreRadius});
        core.setPosition(center);
        core.setFillColor(sf::Color(255, 225, 135, alpha));
        window.draw(core);

        const float sparkSize = 6.f * (1.f - progress) + 2.f;
        const float sparkDistance = radius * 0.85f * expansion;
        sf::RectangleShape spark({sparkSize, sparkSize});
        spark.setOrigin({sparkSize / 2.f, sparkSize / 2.f});
        spark.setFillColor(sf::Color(255, 165, 60, alpha));
        for (int i = 0; i < 12; ++i)
        {
            const float angle = 2.f * pi * static_cast<float>(i) / 12.f;
            spark.setPosition(center + sf::Vector2f(std::cos(angle), std::sin(angle)) * sparkDistance);
            window.draw(spark);
        }
    }
};
