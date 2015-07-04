#ifndef PLAYER_H
#define  PLAYER_H
#endif

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>


class Player {
public:
  Player(b2World& World);
  ~Player();

  void update();

  b2Body* body;
  int i;
  sf::Sprite sprite;

private:
  //Animation playerAnimation;
};
