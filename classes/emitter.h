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
    m_emitter(0, 0),
    offset(0,0)
    {
      pos= NULL;

      repeatdur = 60;
      for (std::size_t i = 0; i < m_particles.size(); ++i)
      {
          resetParticle(i);
          m_vertices[i].color = sf::Color::White;
      }
    }

    emitter(unsigned int count, int x, int y, int repeat) :
    m_particles(count),
    m_vertices(sf::Quads, count*4),
    m_lifetime(30),
    m_emitter(x, y),
    offset(0,0)
    {
      pos= NULL;
      repeatdur = repeat*count +1;
      // std::cout << repeatdur << std::endl;

      for (std::size_t i = 0; i < m_particles.size(); ++i)
      {

          m_vertices[i].color = sf::Color::White;
      }

    }

    emitter(unsigned int count, int x, int y, const b2Vec2* position) :
    m_particles(count),
    m_vertices(sf::Points, count),
    m_lifetime(60),
    m_emitter(x, y),
    offset(0,0)
    {
      pos = position;
      repeatdur = -1;

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
              if (repeatdur > 0) repeatdur--;
            }

            if (pos) {
              setEmitter(pos->x,pos->y);
            }

            // p.velocity.y += .1;
            // update the position of the corresponding vertex
            m_vertices[i*4].position += p.velocity;
            m_vertices[i*4+1].position += p.velocity;
            m_vertices[i*4+2].position += p.velocity;
            m_vertices[i*4+3].position += p.velocity;
            // std::cout << p.velocity.x << std::endl;

            // update the alpha (transparency) of the particle according to its lifetime
            float ratio = (float)p.lifetime / (float)m_lifetime;
            m_vertices[i*4].color.a = static_cast<sf::Uint8>(ratio * 255);
            m_vertices[i*4+1].color.a = static_cast<sf::Uint8>(ratio * 255);
            m_vertices[i*4+2].color.a = static_cast<sf::Uint8>(ratio * 255);
            m_vertices[i*4+3].color.a = static_cast<sf::Uint8>(ratio * 255);
            // m_vertices[i].color.a = (40/50)*255;

        }
    }


    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = NULL;

        // draw the vertex array
        target.draw(m_vertices, states);
    }


    struct Particle
    {
        sf::Vector2f velocity;
        int lifetime;
    };

//this resetParticle is the big ball one
    // void resetParticle(std::size_t index)
    // {
    //     // give a random velocity and lifetime to the particle
    //     float angle = (std::rand() % 360) * 3.14f / 180.f;
    //     float speed = 10;
    //     // float speed = (std::rand() % 300)/100;
    //     m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    //     m_particles[index].lifetime = m_lifetime;
    //     // m_particles[index].lifetime = m_lifetime - std::rand() % 60;
    //
    //     // reset the position of the corresponding vertex
    //     m_vertices[index].position = m_emitter;
    // }

    void resetParticle(std::size_t index)
    {
        // give a random velocity and lifetime to the particle
        float angle = (std::rand() % 360) * 3.14f / 180.f;
        float speed = .4;
        // float speed = (std::rand() % 300)/100;
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed *3, std::sin(angle) * speed);
        m_particles[index].lifetime = m_lifetime;
        // m_particles[index].lifetime = m_lifetime - std::rand() % 60;

        // reset the position of the corresponding vertex
        int quadsize = 5;
        int x = std::rand() % 10;
        int y = std::rand() % 10;
        m_vertices[index*4].position.x = m_emitter.x+x + quadsize;
        m_vertices[index*4].position.y = m_emitter.y+y + quadsize;
        m_vertices[index*4+1].position.x = m_emitter.x+x +quadsize;
        m_vertices[index*4+1].position.y = m_emitter.y+y -quadsize;
        m_vertices[index*4+2].position.x = m_emitter.x+x -quadsize;
        m_vertices[index*4+2].position.y = m_emitter.y+y -quadsize;
        m_vertices[index*4+3].position.x = m_emitter.x+x -quadsize;
        m_vertices[index*4+3].position.y = m_emitter.y+y +quadsize;
    }

    //offset from the center of the bodies position
    b2Vec2 offset;
    //pointer to the position of a body the emitter can follow
    const b2Vec2* pos;
    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    int m_lifetime;
    int repeatdur;
    sf::Vector2f m_emitter;
};
#endif
