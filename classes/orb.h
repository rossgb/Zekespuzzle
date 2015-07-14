#ifndef ORB_H
#define ORB_H


#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "animation.h"
#include "entity.h"




class Orb: public Entity {
public:
  Orb(b2Body* ref, int x, int y);
  ~Orb();

  void handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix);

  void update(sf::RenderWindow &Window);

  b2Body* body;
  int i;
  sf::Sprite sprite;
  int state;
  int orb;

    //debug
    //debug




};
#endif
