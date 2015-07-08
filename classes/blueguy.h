#ifndef PLAYER_H
#define PLAYER_H
#endif

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "animation.h"

enum BlueStateMask {
    BNONE      = 0x20,
    BLEFT      = 0x1,
    BRIGHT     = 0x2,
    BUP        = 0x4,
    BINAIR     = 0x8,
    BJUMP      = 0x10

};



class BlueGuy {
public:
  BlueGuy(b2World& World, b2Body* playerbod);
  ~BlueGuy();

  b2Body* player;

  void update(sf::RenderWindow &Window);

  b2Body* body;
  int i;
  sf::Sprite sprite;
  int state;

    //debug
    //debug

private:

  animation* rising;
  animation* standing;

  animation* walkLeft;


  void handleState();                             //resolves conflicting states
  void handlePhysics();                           //handles movement based on states
  void handleAnimation(sf::RenderWindow &Window); //gets the texture from the animator based on the state
  animation* currentAnimation;


};