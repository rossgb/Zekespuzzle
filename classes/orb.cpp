#include "orb.h"
#include "iostream"
#include <stdlib.h>
#include <iostream>

static const float SCALE = 30.f;

Orb::Orb(b2Body* ref,int x, int y) {

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody; //this will be a dynamic body
  bodyDef.position.Set(x, y); //set the starting position
  bodyDef.angle = 0; //set the starting angle
  bodyDef.fixedRotation = false;

  b2World* World = ref->GetWorld();


  this->body = World->CreateBody(&bodyDef);
  body->SetUserData(this);

  b2CircleShape circle;
  // circle.m_p.Set(2.0/f, 3.0f);
  circle.m_radius = (18.f/2)/SCALE;
  b2FixtureDef FixtureDef;
  FixtureDef.density = 0.5f;
  FixtureDef.friction = 0.1f;
  FixtureDef.shape = &circle;
  this->body->CreateFixture(&FixtureDef);

  b2FixtureDef sensor;
  sensor.isSensor = true;
  sensor.shape = &circle;
  sensor.friction = 0.f;
  this->body->CreateFixture(&sensor);

  sf::Texture tex;
  tex.loadFromFile("MainSprites/basic/orb.png");

  sprite.setTexture(tex);
    sprite.setOrigin(18.f, 18.f);

orb=1;

}
Orb::~Orb() {}

void Orb::update(sf::RenderWindow &Window) {
  //handle keyboard
  body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x/1.05,body->GetLinearVelocity().y));
  body->SetAngularVelocity(body->GetAngularVelocity());

  sf::Texture tex;
  tex.loadFromFile("MainSprites/basic/orb.png");

  sprite.setTexture(tex);
    sprite.setOrigin(18.f, 18.f);

sprite.setPosition(SCALE * this->body->GetPosition().x, SCALE * this->body->GetPosition().y);
  sprite.setRotation(this->body->GetAngle() * 180/b2_pi);
  Window.draw(sprite);
}



void Orb::handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix) {

}
