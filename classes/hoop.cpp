#include "hoop.h"
#include "iostream"
#include <stdlib.h>

static const float SCALE = 30.f;

Hoop::Hoop(b2World& World) {


  b2BodyDef hoopDef;
  hoopDef.type = b2_kinematicBody;
  hoopDef.position.Set(5,5);
  hoopDef.angle = 0;
  hoopDef.fixedRotation = true;


  this->body = World.CreateBody(&hoopDef);

  body->SetUserData(this);





}
Hoop::~Hoop() {}

void Hoop::update(sf::RenderWindow &Window) {

}

void Hoop::handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix) {
  // body->ApplyLinearImpulse(b2Vec2(0,-100), body->GetWorldCenter());
  if (BlueGuy* bg = dynamic_cast<BlueGuy*>(other)) {
      // std::cout << "HIT THE DUDE" <<std::endl;
      bg->body->ApplyLinearImpulse(b2Vec2(0,-10),bg->body->GetWorldCenter());
      bg->health--;


  }
}

void Hoop::handleGroundCollision(int begin, b2Fixture* thisFix, b2Fixture* otherFix) {

}
