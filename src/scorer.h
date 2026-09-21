#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <windows.h>

class Scorer
{
private:
    int score = 0;
    sf::Sprite m_spr;
    std::vector<sf::Sprite> num_sprs;

    void divide(){
        num_sprs.clear();
        int score = this->score;
        int bits = 0;
        do{
            int num = score % 10;
            score /= 10;
            m_spr.setTextureRect(sf::IntRect({0 + 8 * num, 30}, {8, 10}));
            m_spr.setPosition({760.f - 15.f * bits, 40.f});
            num_sprs.push_back(m_spr);
            bits++;
        }
        while(score > 0);
    }

public:
    Scorer(const sf::Texture &numbers) : m_spr(numbers) {divide();}

    void draw(sf::RenderWindow &w){
        for(auto it : num_sprs){
            w.draw(it);
        }
    }

    void add(int add){
        this->score += add;
        divide();
    }
    
};
