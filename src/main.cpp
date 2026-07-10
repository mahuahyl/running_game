#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.h"
#include "player.h"
#include "npc.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Running");
    window.setFramerateLimit(240);
    sf::Clock clock;

    // 1. 载入纹理
    sf::Texture p_run_texture, p_idle_texture, npc_run_texture, npc_idle_texture;
    if (!p_run_texture.loadFromFile("assets/Main Characters/Mask Dude/Run (32x32).png") ||
        !p_idle_texture.loadFromFile("assets/Main Characters/Mask Dude/Idle (32x32).png") ||
        !npc_run_texture.loadFromFile("assets/Main Characters/Virtual Guy/Run (32x32).png") ||
        !npc_idle_texture.loadFromFile("assets/Main Characters/Virtual Guy/Idle (32x32).png"))
    {
        return -1;
    }

    Player p1(p_idle_texture, p_run_texture);
    Npc p2(npc_idle_texture, npc_run_texture, &p1);

    while (window.isOpen())
    {

        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
            }
        }

        p1.update(dt);
        p2.update(dt);
        window.clear();
        p1.draw(window);
        p2.draw(window);
        window.display();
    }
}