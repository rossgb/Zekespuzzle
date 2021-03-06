#ifndef ENTITY_H
#define ENTITY_H

#include <Box2D/Box2D.h>

class Entity
{
public:

   virtual void handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix) = 0;
   virtual void handleGroundCollision(int begin, b2Fixture* thisFix, b2Fixture* otherFix) = 0;

   virtual void update(sf::RenderWindow &Window) = 0;


};
#endif
