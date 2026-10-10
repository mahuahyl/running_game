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
        Nenemy_idle_texture, Nenemy_run_texture, Fenemy_idle_texture, Fenemy_run_texture,
        Henemy_idle_texture, Henemy_run_texture,
        bullet_texture, boom_bullet_texture, text_texture, background_texture;
        
    Textures()
    {
        loadtexture(background_texture, "assets/Background/Arena.png");
        loadtexture(p_run_texture, "assets/Main Characters/Mask Dude/Run (32x32).png");
        loadtexture(p_idle_texture, "assets/Main Characters/Mask Dude/Idle (32x32).png");
        loadtexture(npc_run_texture, "assets/Main Characters/Virtual Guy/Run (32x32).png");
        loadtexture(npc_idle_texture, "assets/Main Characters/Virtual Guy/Idle (32x32).png");
        loadtexture(bullet_texture, "assets/Bullets/normal_bullet.png");
        loadtexture(boom_bullet_texture, "assets/Bullets/boom_bullet.png");
        loadtexture(Nenemy_run_texture, "assets/Main Characters/Normal Guard/Run (32x32).png");
        loadtexture(Nenemy_idle_texture, "assets/Main Characters/Normal Guard/Idle (32x32).png");
        loadtexture(Fenemy_run_texture, "assets/Main Characters/Fast Assassin/Run (32x32).png");
        loadtexture(Fenemy_idle_texture, "assets/Main Characters/Fast Assassin/Idle (32x32).png");
        loadtexture(Henemy_idle_texture, "assets/Main Characters/Heavy Knight/Idle (32x32).png");
        loadtexture(Henemy_run_texture, "assets/Main Characters/Heavy Knight/Run (32x32).png");
        loadtexture(text_texture, "assets/Menu/Text/Text (White) (8x10).png");
    }
};
