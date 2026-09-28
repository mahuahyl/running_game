#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cstdlib>
#include "character.h"
#include "player.h"
#include "follower.h"
#include "bullet.h"
#include "enemy.h"
#include "scorer.h"
#include "texter.h"
#include "texture.h"
#include "normal_enemy.h"
#include "fast_enemy.h"

class Game
{
private:
    // 窗口设置
    sf::RenderWindow window;
    sf::Clock clock;

    // 游戏状态机
    enum class game_state
    {
        playing,
        over
    };
    game_state state = game_state::playing;

    // 载入纹理
    Textures textures;

    // 对象创建
    Player p1;
    Follower n1;
    Scorer scorer;
    Texter texter;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    // 敌人生成数据
    float NspawnTimer = 0.f, FspawnTimer = 0.f;
    float NspawnInterval = 5.f, FspawnInterval = 2.f;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> xdist, ydist;

    static float cal_distant(sf::Vector2f p1, sf::Vector2f p2)
    {
        float dx, dy;
        dx = p1.x - p2.x;
        dy = p1.y - p2.y;
        return (dx * dx + dy * dy);
    }

    void reset()
    {
        state = game_state::playing;
        enemies.clear();
        bullets.clear();
        p1.reset();
        scorer.reset();
    }

    void render()
    {
        window.clear();

        // 展示
        for (auto &it : bullets)
        {
            it.draw(window);
        }
        for (auto &it : enemies)
        {
            it.draw(window);
        }
        scorer.draw(window);
        p1.draw(window);
        // n1.draw(window);
        if (state == game_state::over)
        {
            texter.draw({300.f, 150.f}, "press r to replay", window);
        }

        window.display();
    }

    void update(float dt)
    {
        // 敌人生成
        if (state == game_state::playing)
        {
            NspawnTimer += dt;
            FspawnTimer += dt;
            if (NspawnTimer > NspawnInterval)
            {
                sf::Vector2f position;
                sf::Vector2f dis;
                do
                {
                    position = {xdist(gen), ydist(gen)};
                    dis = position - p1.getPosition();

                } while (std::hypot(dis.x, dis.y) < 50.f);

                NormalE e1(textures.Nenemy_idle_texture, textures.Nenemy_run_texture, &p1, position);
                enemies.push_back(e1);
                NspawnTimer -= NspawnInterval;
            }
            if (FspawnTimer > FspawnInterval)
            {
                sf::Vector2f position;
                sf::Vector2f dis;
                do
                {
                    position = {xdist(gen), ydist(gen)};
                    dis = position - p1.getPosition();

                } while (std::hypot(dis.x, dis.y) < 100.f);

                FastE e1(textures.Fenemy_idle_texture, textures.Fenemy_run_texture, &p1, position);
                enemies.push_back(e1);
                FspawnTimer -= FspawnInterval;
            }
        }

        // 敌人和子弹行动
        if (state == game_state::playing)
        {
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
                    if (cal_distant(b_position, e_position) <= 10 * 10)
                    {
                        e_it.Heated();
                        b_it.kill();
                    }
                }
            }

            for (auto &e_it : enemies)
            {
                sf::Vector2f e_position = e_it.getPosition();
                sf::Vector2f p_position = p1.getPosition();
                if (cal_distant(e_position, p_position) <= 20 * 20)
                {
                    state = game_state::over;
                }
            }

            for (auto it = enemies.begin(); it != enemies.end();)
            {
                if (it->check_if_dead())
                {
                    scorer.add(200);
                    it = enemies.erase(it);
                }
                else
                {
                    it++;
                }
            }

            for (auto it = bullets.begin(); it != bullets.end();)
            {
                if (it->check_distance() || !it->check_is_alive())
                {
                    it = bullets.erase(it);
                }
                else
                {
                    it++;
                }
            }

            p1.update(dt);
            // n1.update(dt);
        }
    }

    void processEvents()
    {
        // 事件检测
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (state == game_state::playing)
            {
                if (const auto *mouse = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouse->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        sf::Vector2f b_dire = mouse_pos - p1.getPosition();

                        bullets.emplace_back(textures.bullet_texture, b_dire, p1.getPosition());
                    }
                }

                if (const auto *key = event->getIf<sf::Event::KeyPressed>())
                {
                    if (key->code == sf::Keyboard::Key::P)
                    {
                        NormalE e1(textures.Nenemy_idle_texture, textures.Nenemy_run_texture, &p1, {100.f, 200.f});
                        enemies.push_back(e1);
                    }
                }
            }

            if (state == game_state::over)
            {
                if (const auto *key = event->getIf<sf::Event::KeyPressed>())
                {
                    if (key->code == sf::Keyboard::Key::R)
                    {
                        reset();
                    }
                }
            }
        }
    }

public:
    Game() : window(sf::VideoMode({800, 600}), "Running"),
             p1(textures.p_idle_texture, textures.p_run_texture),
             n1(textures.npc_idle_texture, textures.npc_run_texture, &p1),
             scorer(textures.text_texture),
             texter(textures.text_texture),
             gen(rd()),
             ydist(0.f, 600.f),
             xdist(0.f, 800.f)
    {
        window.setFramerateLimit(240);
    }
    void run()
    {

        // 进程
        while (window.isOpen())
        {
            // 记录时间
            float dt = clock.restart().asSeconds();
            if (dt > 0.1f) dt = 0.1f;
            update(dt);
            processEvents();
            render();
        }
    }
};