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
#include "scorer.h"

float cal_distant(sf::Vector2f p1, sf::Vector2f p2)
{
    float dx, dy;
    dx = p1.x - p2.x;
    dy = p1.y - p2.y;
    return (dx * dx + dy * dy);
}

int main()
{
    // 窗口设置
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Running");
    window.setFramerateLimit(240);
    sf::Clock clock;
    float spawnTimer = 0.f;
    float spawnInterval = 5.f;

    // 随机数
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> ydist(0.f, 600.f);
    std::uniform_real_distribution<float> xdist(0.f, 800.f);

    // 游戏状态机
    enum class game_state
    {
        playing,
        over
    };
    game_state state = game_state::playing;

    // 1. 载入纹理
    sf::Texture p_run_texture, p_idle_texture, npc_run_texture, npc_idle_texture, enemy_idle_texture, enemy_run_texture, bullet_texture, text_texture;
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
    if (!text_texture.loadFromFile("assets/Menu/text/Text (White) (8x10).png"))
    {
        MessageBoxA(NULL, "Failed to load: text texture", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // 对象创建
    Player p1(p_idle_texture, p_run_texture);
    Follower n1(npc_idle_texture, npc_run_texture, &p1);
    Scorer scorer(text_texture);
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    // 进程
    while (window.isOpen())
    {
        // 记录时间
        float dt = clock.restart().asSeconds();

        // 敌人生成
        if (state == game_state::playing)
        {
            spawnTimer += dt;
            if (spawnTimer > spawnInterval)
            {
                sf::Vector2f position;
                sf::Vector2f dis;
                do
                {

                    position = {xdist(gen), ydist(gen)};
                    dis = position - p1.getPosition();

                } while (std::hypot(dis.x, dis.y) < 50.f);

                Enemy e1(enemy_idle_texture, enemy_run_texture, &p1, position);
                enemies.push_back(e1);
                spawnTimer -= spawnInterval;
            }
        }

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

                        bullets.emplace_back(bullet_texture, b_dire, p1.getPosition());
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

        window.clear();

        if (state == game_state::over)
        {
        }

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
        window.display();
    }
}