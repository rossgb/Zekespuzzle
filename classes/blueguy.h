#ifndef BLUEGUY_H
#define BLUEGUY_H


#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "animation.h"
#include "orb.h"
#include "entity.h"

enum BlueStateMask {
    BNONE      = 0x20,
    BLEFT      = 0x1,
    BRIGHT     = 0x2,
    BUP        = 0x4,
    BINAIR     = 0x8,
    BJUMP      = 0x10,
    BDIE       = 0x40,
    BDEAD      = 0x80

};



class BlueGuy: public Entity {
public:
  BlueGuy(b2World& World, b2Body* playerbod);
  ~BlueGuy();

  void handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix);


  b2Body* player;

  void update(sf::RenderWindow &Window);


  b2Body* body;
  int health;
  int i;
  sf::Sprite sprite;
  int state;

    //debug
    //debug

private:

  animation* rising;
  animation* standing;

  animation* walkLeft;
  animation* die;


  void handleState();                             //resolves conflicting states
  void handlePhysics();                           //handles movement based on states
  void handleAnimation(sf::RenderWindow &Window); //gets the texture from the animator based on the state
  animation* currentAnimation;


};
#endif
