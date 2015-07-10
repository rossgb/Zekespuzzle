#ifndef ENTITY_H
#define ENTITY_H


class Entity
{
public:

   virtual void handleCollision(Entity* other, int begin) = 0;


};
#endif
