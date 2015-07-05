#include "player.h"
#include "iostream"
#include <stdlib.h>

static const float SCALE = 30.f;

Player::Player(b2World& World) {

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody; //this will be a dynamic body
  bodyDef.position.Set(5, 5); //set the starting position
  bodyDef.angle = 0; //set the starting angle
  bodyDef.fixedRotation = true;

  hoopfront.loadFromFile("MainSprites/basic/hoopfront.png");
  hoopback.loadFromFile("MainSprites/basic/hoopback.png");



  rising = new animation("jump1");
  falling = new animation("jump2");
  standing = new animation("idle");
  hanging = new animation("hang");
   walkLeft = new animation("walk/walkframes", 10, 6);
   walkRight = new animation("walk/walkframes", 10, 6);
   slashLeft = new animation("slash/slashframes", 4, 6);
   slashRight = new animation("slash/slashframes", 4, 6);
   slashRight = new animation("boom/boomframes", 9, 4);

currentAnimation = walkLeft;
currentAnimation->start();

  this->body = World.CreateBody(&bodyDef);

  b2PolygonShape Shape;
  Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 0.7f;
  FixtureDef.shape = &Shape;
  this->body->CreateFixture(&FixtureDef);
  this->state = NONE;
  lastx=SCALE * this->body->GetPosition().x;
  lasty=SCALE * this->body->GetPosition().y;

}
Player::~Player() {}

void Player::update(sf::RenderWindow &Window) {
  //handle keyboard
  currentAnimation->update();

  handleKeyboard(Window);

  handlePhysics();

  handleState();

  //get textures from animator and render it to the players position
  handleAnimation(Window);

}


void Player::handlePhysics() {
    std::cout << state << std::endl;
    if (this->body->GetLinearVelocity().y == 0 && (state & INAIR)) {
        std::cout << "landed";
        state -= INAIR;
    }

    if (abs(this->body->GetLinearVelocity().y) > 0.1 && !(state & INAIR)) {
        std::cout << "un-landed";
        state += INAIR;
    }

    if (state & INAIR) {
      if (this->body->GetLinearVelocity().y > 0) {

      } else {

      }
    }

    if (!(this->state & INAIR) && (this->state & UP)) {
        std::cout << "jump" << std::endl;
        this->state += INAIR;
        this->body->ApplyLinearImpulse( b2Vec2(0,-17), this->body->GetWorldCenter());
    }
}

void Player::handleAnimation(sf::RenderWindow &Window) {
  hoopbacksp;
  hoopbacksp.setTexture(hoopback);
  hoopbacksp.setOrigin(81.f,25.f);
  hoopbacksp.setPosition(lastx,lasty);
  if (state&LEFT) {
    hoopbacksp.setScale(-1.f,1.f);
  }
  if (state&RIGHT) {
    hoopbacksp.setScale(1.f,1.f);
  }

  Window.draw(hoopbacksp);

  if (state&INAIR && this->body->GetLinearVelocity().y<0) {
    currentAnimation = rising;
  } else if (state&INAIR && this->body->GetLinearVelocity().y>0) {
    currentAnimation = falling;
  } else {
    currentAnimation = walkLeft;
  }
  sprite.setTexture(*currentAnimation->getTexture());
  sprite.setOrigin(30.f, 30.f);
  sprite.setPosition(SCALE * this->body->GetPosition().x, SCALE * this->body->GetPosition().y);
  sprite.setRotation(this->body->GetAngle() * 180/b2_pi);
  if (state&LEFT) {
    sprite.setScale(-1.f,1.f);
  }
  if (state&RIGHT) {
    sprite.setScale(1.f,1.f);
  }

  Window.draw(sprite);

  hoopfrontsp;
  hoopfrontsp.setTexture(hoopfront);
  hoopfrontsp.setOrigin(81.f,25.f);
  hoopfrontsp.setPosition(lastx,lasty);
  if (state&LEFT) {
    hoopfrontsp.setScale(-1.f,1.f);
  }
  if (state&RIGHT) {
    hoopfrontsp.setScale(1.f,1.f);
  }
  Window.draw(hoopfrontsp);

  lastx=SCALE * this->body->GetPosition().x;
  lasty=SCALE * this->body->GetPosition().y;

}

void Player::handleState() {

}

void Player::handleKeyboard(sf::RenderWindow &Window) {
  sf::Event event;

  while (Window.pollEvent(event))
  {
      // check the type of the event...
      switch (event.type)
      {
          // key pressed
          case sf::Event::KeyPressed:
              if (event.key.code == sf::Keyboard::Left)
              {
                  this->state += LEFT;
              }
              if (event.key.code == sf::Keyboard::Right)
              {
                  this->state += RIGHT;
              }
              if (event.key.code == sf::Keyboard::Up)
              {
                  this->state += UP;
              }
              if (event.key.code == sf::Keyboard::Down)
              {
                  this->state += DOWN;
              }
              if (event.key.code == sf::Keyboard::Space)
              {
                  this->state += SPACE;
              }
              break;
          case sf::Event::KeyReleased:
              if (event.key.code == sf::Keyboard::Left)
              {
                  this->state -= LEFT;
              }
              if (event.key.code == sf::Keyboard::Right)
              {
                  this->state -= RIGHT;
              }
              if (event.key.code == sf::Keyboard::Up)
              {
                  this->state -= UP;
              }
              if (event.key.code == sf::Keyboard::Down)
              {
                  this->state -= DOWN;
              }
              if (event.key.code == sf::Keyboard::Space)
              {
                  this->state -= SPACE;
              }
              break;


          // we don't process other types of events
          default:
              break;
      }
  }
}
