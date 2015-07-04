#ifndef PLAYER_H
#define  PLAYER_H
#endif

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "animation.h"


class Player {
public:
  Player(b2World& World);
  ~Player();

  void update();

  b2Body* body;
  int i;
  sf::Sprite sprite;


private:
  sf::Texture rising;
  sf::Texture falling;
  sf::Texture standing;
  sf::Texture hanging;


  animation walkLeft();
  animation walkRight();
  animation slashLeft();
  animation slashRight();
  animation nova();

  //Animation playerAnimation;
};
