#ifndef PARTICLESYSTEM_H
#define  PARTICLESYSTEM_H


#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>
#include "emitter.h"


class particlesystem
{
public:

    particlesystem(sf::RenderWindow* win)
    {
      Window = win;
    }


    void reset()
    {
        m_emitters.clear();
    }

    void add(emitter e)
    {
      m_emitters.push_back(e);

    }

    void update()
    {

        for (int i = 0; i < m_emitters.size(); )
        {



            m_emitters[i].update();
            if (m_emitters[i].repeatdur != 0) {
              Window->draw(m_emitters[i]);

            }

            if (m_emitters[i].repeatdur == 0) {

              m_emitters.erase(m_emitters.begin() + i);
            } else {

              i++;
            }
            // i++;

        }
    }



    sf::RenderWindow* Window;
    std::vector<int> deletelist;
    std::vector<emitter> m_emitters;
};
#endif
