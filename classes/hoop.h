#ifndef HOOP_H
#define HOOP_H


#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "entity.h"
#include "blueguy.h"




class Hoop: public Entity {
public:
  Hoop(b2World& World);
  ~Hoop();

  void handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix);
  void handleGroundCollision(int begin, b2Fixture* thisFix, b2Fixture* otherFix);

  void update(sf::RenderWindow &Window);

  b2Body* body;
  b2Fixture* hoopfx;

    //debug
private:


};
#endif
