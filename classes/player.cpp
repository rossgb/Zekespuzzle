#include "player.h"
#include "iostream"
#include <stdlib.h>

static const float SCALE = 30.f;

Player::Player(b2World& World) {
  hoopfront.loadFromFile("MainSprites/basic/hoopfront.png");
  hoopback.loadFromFile("MainSprites/basic/hoopback.png");
  hoopflat.loadFromFile("MainSprites/basic/hoopflat.png");

  rising = new animation("jump1");
  falling = new animation("jump2");
  standing = new animation("idle");
  hanging = new animation("hang");
  walkLeft = new animation("walk/walkframes", 10, 6);
  walkRight = new animation("walk/walkframes", 10, 6);
  slashLeft = new animation("slash/slashframes", 4, 6);
  slashRight = new animation("slash/slashframes", 4, 6);
  nova = new animation("boom/boomframes", 9, 4);

  hooporigin = b2Vec2(0.f,0.f);


  currentAnimation = standing;
  currentAnimation->start();

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody; //this will be a dynamic body
  bodyDef.position.Set(5, 5); //set the starting position
  bodyDef.angle = 0; //set the starting angle
  bodyDef.fixedRotation = true;

  b2BodyDef hoopDef;
  hoopDef.type = b2_kinematicBody;
  hoopDef.position.Set(5,5);
  hoopDef.angle = 0;
  hoopDef.fixedRotation = true;
  hoopDef.linearDamping = 0.f;

  this->body = World.CreateBody(&bodyDef);
  this->hoop = World.CreateBody(&hoopDef);

  b2PolygonShape Shape;
  Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 5.f;
  FixtureDef.shape = &Shape;
  this->body->CreateFixture(&FixtureDef);

  b2CircleShape circle;
  // circle.m_p.Set(2.0f, 3.0f);
  circle.m_radius = (32.f/2)/SCALE;
  b2FixtureDef hoopFixDef;
  hoopFixDef.density = 0.f;
  hoopFixDef.friction = 0.f;
  hoopFixDef.shape = &circle;
  hoopFixDef.isSensor = true;
  this->hoop->CreateFixture(&hoopFixDef);



  this->state = NONE;
  lastx=SCALE * this->body->GetPosition().x;
  lasty=SCALE * this->body->GetPosition().y;

  counter = 0;
  debug = true;
  novacounter = 0;
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
  if (debug)  debugPrints();

}


void Player::handlePhysics() {

    //if the velocity hits zero and the character is in the air, ground and add a jump
    if (this->body->GetLinearVelocity().y == 0 && (state & INAIR)) {
        state -= INAIR;
        if (state&JUMPED) {
          state -= JUMPED;
        }
    }

    //if y velocity is there then character is in the air
    if (abs(this->body->GetLinearVelocity().y) > 0.1 && !(state & INAIR)) {
        state += INAIR;
    }

    //walking force
    int force = 50;
    //reduce control in air/while attacking
    if (state & INAIR || state&SPACE || state&NOVA) {
      force /=4;
    }
    if (state & LEFT) {
      this->body->ApplyForce( b2Vec2(-force,0), this->body->GetWorldCenter() );
    }
    if (state & RIGHT) {
      this->body->ApplyForce( b2Vec2(force,0), this->body->GetWorldCenter() );
    }

    //hover velocity
    if (state&NOVA) {
      this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x/1.2,-0.9));
    }

    //only allow jump to be pressed when not in NOVA
    if (!(state&NOVA)) {
      if (!(state & INAIR) && (state & UP)) {
          state += INAIR;
          this->body->ApplyLinearImpulse( b2Vec2(0,-17), this->body->GetWorldCenter());
          state -= UP;
      } else if ((state & INAIR) && (state & UP) && !(state & JUMPED)) {
          this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x,-0.1));
          this->body->ApplyLinearImpulse( b2Vec2(0,-17), this->body->GetWorldCenter());
          state -= UP;
          state += JUMPED;
      } else if (state & UP) {
          state -= UP;
      }
    }

    //limit horizontal velocity
    if (this->body->GetLinearVelocity().x>10) {
      this->body->SetLinearVelocity(b2Vec2(10,this->body->GetLinearVelocity().y));
    }

    if (this->body->GetLinearVelocity().x<-10) {
      this->body->SetLinearVelocity(b2Vec2(-10,this->body->GetLinearVelocity().y));
    }

    //HOOP STUFF
    //follow the character around
    if (!(state&THROWN)) {
      hoop->SetTransform(b2Vec2(lastx,lasty),0);
      hooporigin.x = lastx;
      hooporigin.y = lasty;
    } else {
      hoop->SetLinearVelocity(b2Vec2(50000,0));
    }

    if (abs((hoop->GetPosition().x)-hooporigin.x) > 500 && (state&THROWN)) {
      state -= THROWN;
      // state += RETURN;
    }
}

void Player::handleAnimation(sf::RenderWindow &Window) {
  hoopbacksp;
  if (state&THROWN || state&RETURN) {
    hoopbacksp.setTexture(hoopflat);
  } else {
    hoopbacksp.setTexture(hoopback);
  }
  hoopbacksp.setOrigin(81.f,25.f);
  hoopbacksp.setPosition(hoop->GetPosition().x,hoop->GetPosition().y);
  if (state&LEFT && !(state&THROWN)) {
    hoopbacksp.setScale(-1.f,1.f);
  }
  if (state&RIGHT && !(state&THROWN)) {
    hoopbacksp.setScale(1.f,1.f);
  }
  if(!(state&SPACE) && !(state&NOVA)) {
    Window.draw(hoopbacksp);
  }


  //MAIN PLAYER
  if (state&NOVA) {
    currentAnimation=nova;
    currentAnimation->start();
  }
  if (state&NOVA && (currentAnimation->currentFrame==1) && (currentAnimation->frameCount==3)) {
    if (state&X) {
      currentAnimation->stop();
      novacounter++;
      if (novacounter > 60) {
        novacounter = 0;
        state -= X;
      }
      if (!(state&POSED)) state += POSED;
    } else {
      novacounter = 0;
      currentAnimation->reset();
      slashLeft->reset();
      this->state -= NOVA;
    }

  }

  if (!(state&NOVA) && !(state&X) && state&POSED && !(state&THROWN)) {
    state -= POSED;
    state += THROWN;
  }
  if (state&SPACE && !(state&NOVA)) {
    currentAnimation=slashLeft;
    currentAnimation->start();
  }
  if (state&SPACE && (currentAnimation->currentFrame==1) && (currentAnimation->frameCount==5)) {
    currentAnimation->reset();
    this->state -= SPACE;

  }
  if (!(state&SPACE) && !(state&NOVA)) {
    if (state&INAIR && this->body->GetLinearVelocity().y<0) {
      currentAnimation = rising;
    } else if (state&INAIR && this->body->GetLinearVelocity().y>0) {
      currentAnimation = falling;
    } else if (!(state & (INAIR|LEFT|RIGHT))) {
      currentAnimation = standing;
    } else {
      currentAnimation = walkLeft;
    }
  }
sprite.setTexture(*currentAnimation->getTexture());
if (state&NOVA) {
  sprite.setOrigin(105.f, 105.f);
  sf::Rect<int> slashrect = sf::Rect<int>(0,0, 210,210);
  sprite.setTextureRect(slashrect);
} else if (state&SPACE && !(state&NOVA)) {
    sprite.setOrigin(132.f, 39.f);
    sf::Rect<int> slashrect = sf::Rect<int>(0,0, 264,66);
    sprite.setTextureRect(slashrect);
  } else {
    sprite.setOrigin(30.f, 39.f);
    sf::Rect<int> slashrect = sf::Rect<int>(0,0, 66,66);
    sprite.setTextureRect(slashrect);
  }
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
  if(!(state&SPACE) && !(state&NOVA) && !(state&THROWN) && !(state&RETURN)) {
    Window.draw(hoopfrontsp);
  }

  lastx=SCALE * this->body->GetPosition().x;
  lasty=SCALE * this->body->GetPosition().y;

}

void Player::handleState() {

}

void Player::debugPrints() {
  counter++;
  if (counter>60) {
    counter = 0;
    std::cout << "novacounter = " << novacounter<< std::endl;
    std::cout << ((counter % 2 == 0) ? "PRINTING STATES" : "PRINTING STATES!") << std::endl;
    std::cout << "LEFT     = " << ((state&LEFT) ? "True" : "False") << std::endl;
    std::cout << "RIGHT    = " << ((state&RIGHT) ? "True" : "False") << std::endl;
    std::cout << "UP       = " << ((state&UP) ? "True" : "False") << std::endl;
    std::cout << "DOWN     = " << ((state&DOWN) ? "True" : "False") << std::endl;
    std::cout << "SPACE    = " << ((state&SPACE) ? "True" : "False") << std::endl;
    std::cout << "INAIR    = " << ((state&INAIR) ? "True" : "False") << std::endl;
    std::cout << "ATTACKING= " << ((state&ATTACKING) ? "True" : "False") << std::endl;
    std::cout << "JUMPED   = " << ((state&JUMPED) ? "True" : "False") << std::endl;
    std::cout << "NOVA     = " << ((state&NOVA) ? "True" : "False") << std::endl;
    std::cout << "X        = " << ((state&X) ? "True" : "False") << std::endl;
    std::cout << "POSED    = " << ((state&POSED) ? "True" : "False") << std::endl;
    std::cout << "THROWN   = " << ((state&THROWN) ? "True" : "False") << std::endl;
    std::cout << "RETURN   = " << ((state&RETURN) ? "True" : "False") << "\n\n" << std::endl;
  }
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
            if (event.key.code == sf::Keyboard::D)
            {
                debug = !debug;
            }
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
                  // this->state += DOWN;
              }
              if (event.key.code == sf::Keyboard::Space && !(state&SPACE) && !(state&NOVA) && !(state&THROWN) && !(state&RETURN))
              {

                  this->state += SPACE;
              }
              if (event.key.code == sf::Keyboard::X && !(state&X) && !(state&THROWN) && !(state&RETURN))
              {

                  if (state&SPACE) {
                    state -= SPACE;
                  }
                  this->state += X;
                  if (!(state&NOVA)) {
                    this->state += NOVA;
                  }
              }
              if (event.key.code == sf::Keyboard::Escape)
              {
                  Window.close();
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
              if (event.key.code == sf::Keyboard::Up && (state&UP))
              {
              }
              if (event.key.code == sf::Keyboard::Down && (state&DOWN))
              {
                  // this->state -= DOWN;
              }
              if (event.key.code == sf::Keyboard::Space)
              {
                  // this->state -= SPACE;
              }
              if (event.key.code == sf::Keyboard::X && (state&X))
              {

                  this->state -= X;
              }
              break;


          // we don't process other types of events
          default:
              break;
      }
  }
}
