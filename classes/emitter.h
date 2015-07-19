#ifndef EMITTER_H
#define  EMITTER_H


#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>


class emitter : public sf::Drawable, public sf::Transformable
{
public:

    emitter(unsigned int count) :
    m_particles(count),
    m_vertices(sf::Points, count),
    m_lifetime(60),
    m_emitter(0, 0)
    {
      for (std::size_t i = 0; i < m_particles.size(); ++i)
      {
          resetParticle(i);
          m_vertices[i].color = sf::Color::White;
      }
    }

    emitter(unsigned int count, int x, int y) :
    m_particles(count),
    m_vertices(sf::Points, count),
    m_lifetime(60),
    m_emitter(x, y)
    {
      for (std::size_t i = 0; i < m_particles.size(); ++i)
      {

          m_vertices[i].color = sf::Color::White;
      }

    }

    void setEmitter(int x,int y)
    {
        m_emitter = sf::Vector2f(x,y);
    }

    void update()
    {
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifetime -= 1;

            // if the particle is dead, respawn it
            if (p.lifetime <= 0) {
              resetParticle(i);
            }

            // p.velocity.y += .1;
            // update the position of the corresponding vertex
            m_vertices[i].position += p.velocity;
            // std::cout << p.velocity.x << std::endl;

            // update the alpha (transparency) of the particle according to its lifetime
            float ratio = p.lifetime / m_lifetime;
            // m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
        }
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = NULL;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

private:

    struct Particle
    {
        sf::Vector2f velocity;
        int lifetime;
    };

    void resetParticle(std::size_t index)
    {
        // give a random velocity and lifetime to the particle
        float angle = (std::rand() % 360) * 3.14f / 180.f;
        float speed = 10;
        // float speed = (std::rand() % 300)/100;
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        m_particles[index].lifetime = m_lifetime;
        // m_particles[index].lifetime = m_lifetime - std::rand() % 60;

        // reset the position of the corresponding vertex
        m_vertices[index].position = m_emitter;
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    int m_lifetime;
    sf::Vector2f m_emitter;
};
#endif
