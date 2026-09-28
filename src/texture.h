#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>

class Textures
{
private:
    static void loadtexture(sf::Texture &tex, const std::string &path)
    {
        if (tex.loadFromFile(path))
            return;
        MessageBoxA(nullptr,
                    ("Failed to load: " + path).c_str(),
                    "Error",
                    MB_OK | MB_ICONERROR);
        std::exit(1);
    }

public:
    sf::Texture p_run_texture, p_idle_texture, npc_run_texture, npc_idle_texture,
        enemy_idle_texture, enemy_run_texture, bullet_texture, text_texture;
        
    Textures()
    {
        loadtexture(p_run_texture, "assets/Main Characters/Mask Dude/Run (32x32).png");
        loadtexture(p_idle_texture, "assets/Main Characters/Mask Dude/Idle (32x32).png");
        loadtexture(npc_run_texture, "assets/Main Characters/Virtual Guy/Run (32x32).png");
        loadtexture(npc_idle_texture, "assets/Main Characters/Virtual Guy/Idle (32x32).png");
        loadtexture(bullet_texture, "assets/Other/Dust Particle.png");
        loadtexture(enemy_run_texture, "assets/Main Characters/Pink Man/Run (32x32).png");
        loadtexture(enemy_idle_texture, "assets/Main Characters/Pink Man/Idle (32x32).png");
        loadtexture(text_texture, "assets/Menu/Text/Text (White) (8x10).png");
    }
};