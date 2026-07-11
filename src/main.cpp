#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "character.h"
#include "player.h"
#include "follower.h"
#include "bullet.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Running");
    window.setFramerateLimit(240);
    sf::Clock clock;

    // 1. 载入纹理
    sf::Texture p_run_texture, p_idle_texture, npc_run_texture, npc_idle_texture, bullet_texture;
    if (!p_run_texture.loadFromFile("assets/Main Characters/Mask Dude/Run (32x32).png"))
    {
        MessageBoxA(NULL, "Failed to load: Run texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (!p_idle_texture.loadFromFile("assets/Main Characters/Mask Dude/Idle (32x32).png"))
    {
        MessageBoxA(NULL, "Failed to load: Idle texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (!npc_run_texture.loadFromFile("assets/Main Characters/Virtual Guy/Run (32x32).png"))
    {
        MessageBoxA(NULL, "Failed to load: NPC Run texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (!npc_idle_texture.loadFromFile("assets/Main Characters/Virtual Guy/Idle (32x32).png"))
    {
        MessageBoxA(NULL, "Failed to load: NPC Idle texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (!bullet_texture.loadFromFile("assets/Other/Dust Particle.png"))
    {
        MessageBoxA(NULL, "Failed to load: Bullet texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    Player p1(p_idle_texture, p_run_texture);
    Follower p2(npc_idle_texture, npc_run_texture, &p1);
    std::vector<Bullet> bullets;

    while (window.isOpen())
    {

        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    bullets.emplace_back(bullet_texture, p1.getFacingDir(), p1.getPosition());
                }
            }
        }

        for (auto& it : bullets)
        {
            it.update(dt);
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet& b) { return b.check_distance(); }), bullets.end());
        p1.update(dt);
        p2.update(dt);
        window.clear();

        for (auto& it : bullets)
        {
            it.draw(window);
        }
        p1.draw(window);
        p2.draw(window);
        window.display();
    }
}