#include "player.h"

Player::Player(b2World& World) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody; //this will be a dynamic body
  bodyDef.position.Set(20, 20); //set the starting position
  bodyDef.angle = 0; //set the starting angle

  this->body = World.CreateBody(&bodyDef);
}
Player::~Player() {}

void Player::update() {
  //handle keyboard
  //handle collisions (with box2d)
  //hangle animator calls
}
