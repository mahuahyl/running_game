#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <random>
#include "character.h"
#include "player.h"
#include "follower.h"
#include "bullet.h"
#include "enemy.h"

int main()
{
    // 窗口设置
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Running");
    window.setFramerateLimit(240);
    sf::Clock clock;
    float spawnTimer = 0.f;
    float spawnInterval = 1.f;

    //随机数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> ydist(0.f, 600.f);
    std::uniform_real_distribution<float> xdist(0.f, 800.f);

    // 1. 载入纹理
    sf::Texture p_run_texture, p_idle_texture, npc_run_texture, npc_idle_texture, enemy_idle_texture, enemy_run_texture, bullet_texture;
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
    if (!enemy_run_texture.loadFromFile("assets/Main Characters/Pink Man/Run (32x32).png"))
    {
        MessageBoxA(NULL, "Failed to load: Enemy Run texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    if (!enemy_idle_texture.loadFromFile("assets/Main Characters/Pink Man/Idle (32x32).png"))
    {
        MessageBoxA(NULL, "Failed to load: Enemy Idle texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // 对象创建
    Player p1(p_idle_texture, p_run_texture);
    Follower n1(npc_idle_texture, npc_run_texture, &p1);
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    // 进程
    while (window.isOpen())
    {
        // 记录时间
        float dt = clock.restart().asSeconds();

        //敌人生成
        spawnTimer += dt;
        if (spawnTimer > spawnInterval)
        {
            sf::Vector2f position = {xdist(gen), ydist(gen)};
            sf::Vector2f dis = position - p1.getPosition();

            while(std::hypot(dis.x, dis.y) < 50.f){
                sf::Vector2f position = {xdist(gen), ydist(gen)};
            }

            Enemy e1(enemy_idle_texture, enemy_run_texture, &p1, position);
            enemies.push_back(e1);
            spawnTimer -= spawnInterval;
        }

        //事件检测
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

            if (const auto *key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::P)
                {
                    Enemy e1(enemy_idle_texture, enemy_run_texture, &p1, {100.f, 200.f});
                    enemies.push_back(e1);
                }
            }
        }

        for (auto &it : bullets)
        {
            it.update(dt);
        }
        for (auto &it : enemies)
        {
            it.update(dt);
        }

        for (auto &b_it : bullets)
        {
            sf::Vector2f b_position = b_it.getPosition();
            for (auto &e_it : enemies)
            {
                sf::Vector2f e_position = e_it.getPosition();
                if ((pow((b_position.x - e_position.x), 2) + pow((b_position.y - e_position.y), 2)) <= 100)
                {
                    e_it.Heated();
                    b_it.kill();
                }
            }
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                     [](Bullet &b)
                                     { return (b.check_distance() || !b.check_is_alive()); }),
                      bullets.end());

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                     [](Enemy &b)
                                     { return b.check_if_dead(); }),
                      enemies.end());

        p1.update(dt);
        // n1.update(dt);
        window.clear();

        for (auto &it : bullets)
        {
            it.draw(window);
        }
        for (auto &it : enemies)
        {
            it.draw(window);
        }

        p1.draw(window);
        // n1.draw(window);
        window.display();
    }
}