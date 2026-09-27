#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>

class Texter
{
private:
    sf::Sprite m_spr;
    std::vector<sf::Sprite> texts;

public:
    Texter(const sf::Texture &texture) : m_spr(texture) {}

    void divide(std::string s)
    {
        texts.clear();
        for (auto it : s)
        {
            if (it >= '0' && it <= '9')
                m_spr.setTextureRect(sf::IntRect({(it - '0') * 8, 30}, {8, 10}));
            else if (it >= 'a' && it <= 'z')
                m_spr.setTextureRect(sf::IntRect({(it - 'a') % 10 * 8, (it - 'a') / 10 * 10}, {8, 10}));
            else if (it == '.')
                m_spr.setTextureRect(sf::IntRect({0, 40}, {8, 10}));
            else if (it == ',')
                m_spr.setTextureRect(sf::IntRect({8, 40}, {8, 10}));
            else if (it == ':')
                m_spr.setTextureRect(sf::IntRect({16, 40}, {8, 10}));
            else if (it == '?')
                m_spr.setTextureRect(sf::IntRect({24, 40}, {8, 10}));
            else if (it == '!')
                m_spr.setTextureRect(sf::IntRect({32, 40}, {8, 10}));
            else if (it == '(')
                m_spr.setTextureRect(sf::IntRect({40, 40}, {8, 10}));
            else if (it == ')')
                m_spr.setTextureRect(sf::IntRect({48, 40}, {8, 10}));
            else if (it == '+')
                m_spr.setTextureRect(sf::IntRect({56, 40}, {8, 10}));
            else if (it == '-')
                m_spr.setTextureRect(sf::IntRect({64, 40}, {8, 10}));
            else
                m_spr.setTextureRect(sf::IntRect({72, 40}, {8, 10}));
            texts.push_back(m_spr);
        }
    }

    void draw(const sf::Vector2f &position, const std::string &s, sf::RenderWindow &w){
        divide(s);
        int num = 0;
        for(auto it : texts){
            it.setPosition({position.x + num * 12, position.y});
            w.draw(it);
            num++;
        }
        texts.clear();
    }

};