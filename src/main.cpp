#include <SFML/Graphics.hpp>
#include <filesystem>
#include <windows.h>
#include "player.h"

// 获取资源绝对路径 — 不管从哪里运行 exe 都能找到 assets/
std::string getAssetPath(const std::string &relativePath)
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    // exe 在 bin/ 下，项目根是上一级
    std::filesystem::path projectRoot =
        std::filesystem::path(exePath).parent_path().parent_path();
    return (projectRoot / relativePath).string();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Running");
    window.setFramerateLimit(60);
    sf::Clock clock;

    // 1. 载入纹理
    sf::Texture texture;
    if (!texture.loadFromFile(getAssetPath("assets/Main Characters/Mask Dude/Run (32x32).png")))
    {
        // 如果载入失败，程序无法继续
        return -1;
    }

    Player p1(texture);

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

        p1.move(dt);
        window.clear();
        p1.draw(window);
        window.display();
    }
}