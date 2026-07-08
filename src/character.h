#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Character
{

public:
    enum class State
    {
        Idle,
        Run
    };

private:
    const sf::Texture &m_idleTex;
    const sf::Texture &m_runTex;
    sf::Sprite m_spr;
    float m_speed = 200.f;
    int m_frame = 0;
    float m_frameTimer = 0.f;
    State m_state = State::Idle;
    int max_frames = 11;

public:
    Character(const sf::Texture &IdleTex, const sf::Texture &RunTex) : m_idleTex(IdleTex), m_runTex(RunTex), m_spr(IdleTex)
    {
        m_spr.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
        auto b = m_spr.getLocalBounds();
        m_spr.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }

    void update(float dt){
        auto direction = Get_direction();

        if(direction.x != 0.f || direction.y != 0.f){
            m_state = State::Run;
            m_spr.setTexture(m_runTex);
            max_frames = 12;

            float len = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
            direction.x = direction.x / len;
            direction.y = direction.y / len;

            m_spr.move({m_speed * direction.x * dt, m_speed * direction.y * dt});
        }else{
            m_state = State::Idle;
            m_spr.setTexture(m_idleTex);
            max_frames = 11;
        }

        m_frameTimer += dt;
        if(m_frameTimer > 0.1f){
            m_frameTimer -= 0.1f;
            m_frame = (m_frame + 1) % max_frames;
            m_spr.setTextureRect(sf::IntRect({m_frame * 32, 0}, {32, 32}));
        }
        
    }

    void draw(sf::RenderWindow &w) const { w.draw(m_spr); }

    virtual sf::Vector2f Get_direction() = 0;
};