#include <SFML/Graphics.hpp>
#include <filesystem>
#include <windows.h>
#include "character.h"
#include "player.h"

// 获取资源绝对路径 — 不管从哪里运行 exe 都能找到 assets/
std::string getAssetPath(const std::string &relativePath)
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::filesystem::path projectRoot =
        std::filesystem::path(exePath).parent_path().parent_path();
    return (projectRoot / relativePath).string();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Running");
    window.setFramerateLimit(240);
    sf::Clock clock;

    // 1. 载入纹理
    sf::Texture p_run_texture, p_idle_texture, npc_run_texture, npc_idle_texture;
    p_run_texture.loadFromFile(getAssetPath("assets/Main Characters/Mask Dude/Run (32x32).png"));
    p_idle_texture.loadFromFile(getAssetPath("assets/Main Characters/Mask Dude/Idle (32x32).png"));
    npc_run_texture.loadFromFile(getAssetPath("assets/Main Characters/Virtual Guy/Run (32x32).png"));
    npc_idle_texture.loadFromFile(getAssetPath("assets/Main Characters/Virtual Guy/Idle (32x32).png"));

    Player p1(p_idle_texture, p_run_texture);

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
        window.clear();
        p1.draw(window);
        window.display();
    }
}