#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include "texter.h"

// 读取游戏的冷却值进行显示，不另设计时器。
class CooldownUI
{
private:
    sf::Sprite icon;
    Texter text;

public:
    CooldownUI(const sf::Texture& bulletTexture, const sf::Texture& textTexture)
        : icon(bulletTexture), text(textTexture)
    {
        icon.setTextureRect(sf::IntRect({0, 0}, {16, 16}));
        icon.setScale({2.f, 2.f});
    }

    void draw(sf::RenderTarget& window, float elapsed, float cooldown, int score, int unlockScore)
    {
        // 跟随视图右上角，给上方的分数留出空间。
        const auto& view = window.getView();
        const auto topLeft = view.getCenter() - view.getSize() / 2.f;
        const sf::Vector2f position = topLeft + sf::Vector2f(view.getSize().x - 240.f, 66.f);
        const bool unlocked = score >= unlockScore;
        const bool ready = unlocked && elapsed >= cooldown;
        const float progress = unlocked
            ? (cooldown > 0.f ? std::clamp(elapsed / cooldown, 0.f, 1.f) : 1.f)
            : (unlockScore > 0 ? std::clamp(static_cast<float>(score) / unlockScore, 0.f, 1.f) : 1.f);
        const sf::Color accent = ready ? sf::Color(255, 213, 100) : sf::Color(111, 119, 146);

        sf::RectangleShape panel({220.f, 100.f});
        panel.setPosition(position);
        panel.setFillColor(ready ? sf::Color(62, 43, 27, 240) : sf::Color(25, 27, 40, 230));
        panel.setOutlineThickness(-2.f);
        panel.setOutlineColor(accent);
        window.draw(panel);

        icon.setPosition(position + sf::Vector2f(10.f, 12.f));
        icon.setColor(ready ? sf::Color::White : sf::Color(120, 120, 130));
        window.draw(icon);
        text.draw(position + sf::Vector2f(54.f, 12.f), "blast", window, accent);

        if (!unlocked)
        {
            text.draw(position + sf::Vector2f(54.f, 31.f), "locked", window, accent);
        }
        else if (ready)
        {
            text.draw(position + sf::Vector2f(54.f, 31.f), "ready!", window, accent);
        }
        else
        {
            // 向上取整到 0.1 秒，避免未就绪时显示 0.0。
            const float remaining = std::max(0.f, cooldown - elapsed);
            char countdown[32];
            std::snprintf(countdown, sizeof(countdown), "%.1fs", std::ceil(remaining * 10.f) / 10.f);
            text.draw(position + sf::Vector2f(54.f, 31.f), countdown, window, sf::Color(184, 189, 205));
        }

        sf::RectangleShape track({196.f, 6.f});
        track.setPosition(position + sf::Vector2f(12.f, 57.f));
        track.setFillColor(sf::Color(12, 14, 22));
        window.draw(track);
        sf::RectangleShape fill({196.f * progress, 6.f});
        fill.setPosition(track.getPosition());
        fill.setFillColor(accent);
        window.draw(fill);

        if (!unlocked)
        {
            char hint[48];
            std::snprintf(hint, sizeof(hint), "unlock at %d", unlockScore);
            text.draw(position + sf::Vector2f(12.f, 78.f), hint, window, accent, 0.75f);
        }
        else
        {
            text.draw(position + sf::Vector2f(12.f, 78.f),
                      ready ? "right click to fire" : "recharging...", window, accent, 0.75f);
        }
    }
};
