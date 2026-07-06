#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <windows.h>
#include <cmath>

class Player
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

public:
    Player(const sf::Texture &IdleTex, const sf::Texture &RunTex) : m_idleTex(IdleTex), m_runTex(RunTex), m_spr(IdleTex)
    {
        m_spr.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
        auto b = m_spr.getLocalBounds();
        m_spr.setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }

    void move(float dt)
    {
        float x = 0, y = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            x += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            x -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            y -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            y += 1.f;
        }

        float len = std::sqrt(x * x + y * y);
        if (len > 0.f)
        {
            m_state = State::Run;
            m_spr.setTexture(m_runTex);
            x /= len;
            y /= len;
            m_spr.move({m_speed * x * dt, m_speed * y * dt});
        }
        else
        {
            m_state = State::Idle;
            m_spr.setTexture(m_idleTex);
        }

        m_frameTimer += dt;
        if (m_frameTimer > 0.1f)
        {
            m_frameTimer -= 0.1f;
            int maxFrames = (m_state == State::Idle) ? 11 : 12;
            m_frame = (m_frame + 1) % maxFrames;
            m_spr.setTextureRect(sf::IntRect({m_frame * 32, 0}, {32, 32}));
        }
    }

    void draw(sf::RenderWindow &w) const { w.draw(m_spr); }

};