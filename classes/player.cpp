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


  this->body = World.CreateBody(&bodyDef);

  body->SetUserData(this);

  this->hoop = World.CreateBody(&hoopDef);


  b2PolygonShape Shape;
  Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 5.f;
  FixtureDef.shape = &Shape;
  FixtureDef.filter.categoryBits = 2;

  this->body->CreateFixture(&FixtureDef);

  b2CircleShape circle;
  // circle.m_p.Set(2.0/f, 3.0f);
  circle.m_radius = (32.f/2)/SCALE;
  b2FixtureDef hoopFixDef;
  hoopFixDef.density = 0.f;
  hoopFixDef.friction = 0.f;
  hoopFixDef.shape = &circle;
  hoopFixDef.isSensor = false;
  hoopFixDef.filter.maskBits = 0xFFFF-2;
  hoopFixDef.filter.categoryBits = 8;

  this->hoop->CreateFixture(&hoopFixDef);
  hoop->SetUserData( this);



  state = NONE;
  lastx=SCALE * this->body->GetPosition().x;
  lasty=SCALE * this->body->GetPosition().y;

  //debug
  counter = 0;
  debug = false;
  //debug

  health = 100;
  canBeDamaged = true;
  dmgcounter = 0;

  //this is to count the number of frames nova hover lasts
  novacounter = 0;
  //the direction the hoop is thrown
  hoopdir = 0;
}
Player::~Player() {}

void Player::update(sf::RenderWindow &Window) {
  //handle keyboard
  currentAnimation->update();

  handleKeyboard(Window);

  handleState();

  handlePhysics();

  if (!canBeDamaged) {
    dmgcounter++;
  }
  if (dmgcounter >60) {
    dmgcounter = 0;
    canBeDamaged = true;
  }

  //get textures from animator and render it to the players position
  handleAnimation(Window);
  if (debug) debugPrints();

}

void Player::handleCollision(Entity* other, int begin) {
  // body->ApplyLinearImpulse(b2Vec2(0,-100), body->GetWorldCenter());
  if (BlueGuy* bg = (BlueGuy*)other)
  std::cout << "HIT THE DUDE" <<std::endl;
}


void Player::handlePhysics() {

    //if the velocity hits zero and the character is in the air, ground and add a jump
    if (this->body->GetLinearVelocity().y == 0) {
        state |= INAIR;
        state -= INAIR;

        //reset all jumps
        state |= JUMP1;
        state |= JUMP2;
        state |= JUMPED;
        state -= JUMP2;
        state -= JUMP1;
        state -= JUMPED;
        state |= CANNOVA; // can nova once per jump
    }

    //if y velocity is there then character is in the air
    if (abs(this->body->GetLinearVelocity().y) > 0.1 && !(state & INAIR)) {
        state |= INAIR;
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
      this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x/1.1,-1.4));
    }

    //if not nova
      //if jump1 is false
        //first jump
      //if jump1 is true and jump2 is false
        //second jump
    if (!(state&NOVA) && state & C) {
        if (!(state & JUMP1)) {
            state |= JUMP1;
            this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x,-0.1));
            this->body->ApplyLinearImpulse( b2Vec2(0,-16), this->body->GetWorldCenter());
        } else if (!(state & JUMP2) && state & JUMPED) {
            state -= JUMPED;
            state |= JUMP2;
            this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x,-0.1));
            this->body->ApplyLinearImpulse( b2Vec2(0,-16), this->body->GetWorldCenter());
        }
    }

    //
    // //only allow jump to be pressed when not in NOVA
    // if (!(state&NOVA)) {
    //   if (!(state & INAIR) && (state & JUMP1)) {
    //       state |= INAIR;
    //       this->body->ApplyLinearImpulse( b2Vec2(0,-14), this->body->GetWorldCenter());
    //       state -= JUMP1;
    //   } else if ((state & INAIR) && (state & JUMP1) && !(state & JUMPED)) {
    //       this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x,-0.1));
    //       this->body->ApplyLinearImpulse( b2Vec2(0,-14), this->body->GetWorldCenter());
    //       state -= JUMP1;
    //       state |= JUMPED;
    //   } else if (state & JUMP1) {
    //       state -= JUMP1;
    //   }
    // }

    //limit horizontal velocity
    if (this->body->GetLinearVelocity().x>10) {
      this->body->SetLinearVelocity(b2Vec2(10,this->body->GetLinearVelocity().y));
    }

    if (this->body->GetLinearVelocity().x<-10) {
      this->body->SetLinearVelocity(b2Vec2(-10,this->body->GetLinearVelocity().y));
    }


    //-------HOOP STUFF-----------
    //detect when the returning hoop is on the character to catch it
    if (b2Distance(body->GetPosition(),hoop->GetPosition())<0.9 && state&RETURN) {
      state -= RETURN;
    }

    //follow the character around
    if (!(state&THROWN) && !(state&RETURN)) {
      hoop->SetTransform(b2Vec2(lastx/SCALE,lasty/SCALE),0);
      hooporigin.x = lastx/SCALE;
      hooporigin.y = lasty/SCALE;
    } else if (state&RETURN) {  //the hoop velocity returning to the character
      b2Vec2 h = hoop->GetPosition();
      b2Vec2 b = body->GetPosition();
      b2Vec2 dir = b2Vec2(b.x-h.x,b.y-h.y);
      dir.Normalize(); dir.x *= 13; dir.y *= 13;
      hoop->SetLinearVelocity(b2Vec2((b.x-h.x)*5,(b.y-h.y)*5)+(dir));
    } else { //the velocity when the hoop is thrown
      switch (hoopdir){
        case 0:
        this->hoop->SetLinearVelocity(b2Vec2(20,-0.1)); break;
        // hoop->SetTransform(b2Vec2(hoop->GetPosition().x+8,hoop->GetPosition().y),0);
        case 1:this->hoop->SetLinearVelocity(b2Vec2(20,20)); break;
        case 2:this->hoop->SetLinearVelocity(b2Vec2(0,20)); break;
        case 3:this->hoop->SetLinearVelocity(b2Vec2(-20,20)); break;
        case 4:this->hoop->SetLinearVelocity(b2Vec2(-20,-.1)); break;
        case 5:this->hoop->SetLinearVelocity(b2Vec2(-20,-20)); break;
        case 6:this->hoop->SetLinearVelocity(b2Vec2(0,-20)); break;
        case 7:this->hoop->SetLinearVelocity(b2Vec2(20,-20)); break;
        case 8:state -= THROWN; break;
      }
    }

    //if the hoop has travelled sufficient distance, start returning
    if (((hoop->GetPosition().x)-hooporigin.x)*((hoop->GetPosition().x)-hooporigin.x) + ((hoop->GetPosition().y)-hooporigin.y)*((hoop->GetPosition().y)-hooporigin.y) > 200 && (state&THROWN)) {
      if(state&CATCH) {
        state -= CATCH;
        switch (hoopdir){
          case 0:
          this->body->SetLinearVelocity(b2Vec2(10,-0.1)); break;
          // hoop->SetTransform(b2Vec2(hoop->GetPosition().x+8,hoop->GetPosition().y),0);
          case 1:this->body->SetLinearVelocity(b2Vec2(10,10)); break;
          case 2:this->body->SetLinearVelocity(b2Vec2(0,10)); break;
          case 3:this->body->SetLinearVelocity(b2Vec2(-10,10)); break;
          case 4:this->body->SetLinearVelocity(b2Vec2(-10,-.1)); break;
          case 5:this->body->SetLinearVelocity(b2Vec2(-10,-10)); break;
          case 6:this->body->SetLinearVelocity(b2Vec2(0,-10)); break;
          case 7:this->body->SetLinearVelocity(b2Vec2(10,-10)); break;
          case 8:state -= THROWN; break;
        }
      }
      state -= THROWN;
      state |= RETURN;

    }

    if (state&THROWN && state&X) {
      // state |= CATCH;
    }
    // if (state&CATCH && !(state&X)) {
    //   state -= CATCH;
    //   state |= RETURN;
    // }

    if (state&CATCH) {
      body->SetLinearVelocity(hoop->GetLinearVelocity());
    }
}

void Player::handleAnimation(sf::RenderWindow &Window) {
  //the back part of the hoop is also used as the thrown hoop sprite
  hoopbacksp;
  if (state&THROWN || state&RETURN) {
    hoopbacksp.setTexture(hoopflat);
  } else {
    hoopbacksp.setTexture(hoopback);
  }
  if (state&THROWN || state&RETURN) {
  hoopbacksp.setOrigin(61.f,57.f);
  } else {
  hoopbacksp.setOrigin(81.f,25.f);
  }

  if (state&THROWN || state&RETURN) {
  hoopbacksp.setTextureRect(sf::Rect<int>(0,0,123,114));
  } else {
    hoopbacksp.setTextureRect(sf::Rect<int>(0,0,160,66));
  }

  hoopbacksp.setPosition(SCALE * hoop->GetPosition().x,SCALE * hoop->GetPosition().y);
  if (state&LEFT) {
    hoopbacksp.setScale(-1.f,1.f);
  }
  if (state&RIGHT) {
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
  //at the end of the nova animation, allow the player to stay on the last frame for a short time while holding X
  //also gives time to aim the hoop before it's thrown
  // if (state&NOVA && (currentAnimation->currentFrame==1) && (currentAnimation->frameCount==3)) {
  if (state&NOVA && (currentAnimation->currentFrame==2||currentAnimation->currentFrame==3) && state&X) {
    if (!(state&POSED)) state |= POSED;
  }

  if (state&NOVA && (currentAnimation->currentFrame==1)) {
    if (state&X) {
      currentAnimation->stop();
      novacounter++;
      if (novacounter > 30) {
        novacounter = 0;
        state -= NOVA;
        currentAnimation->reset();
      }
      //when the last frame is reached, if X is still pressed, get the hoop ready to throw (POSED state)
      if (!(state&POSED)) state |= POSED;
    } else { //the animation ended without X being held
      novacounter = 0;
      currentAnimation->reset();
      slashLeft->reset();
      state -= NOVA;
    }

  }

  //if the hoop was posed and nova is done, decide which direction to throw the hoop
  //and change states
  if (!(state&NOVA) && state&POSED && !(state&THROWN)) {
    state -= POSED;
    state |= THROWN;
    if (state&RIGHT) {
      hoopdir = 0;
      if (state&UP) {
        hoopdir = 7;
      }
      if (state&DOWN) {
        hoopdir = 1;
      }
    } else if (state&LEFT) {
      hoopdir = 4;
      if (state&UP) {
        hoopdir = 5;
      }
      if (state&DOWN) {
        hoopdir = 3;
      }
    } else {
      hoopdir = 8;
      if (state&UP) {
        hoopdir = 6;
      }
      if (state&DOWN) {
        hoopdir = 2;
      }
    }
  }


  //basic slash animation
  if (state&SPACE && !(state&NOVA)) {
    currentAnimation=slashLeft;
    currentAnimation->start();
  }
  if (state&SPACE && (currentAnimation->currentFrame==1) && (currentAnimation->frameCount==5)) {
    currentAnimation->reset();
    state -= SPACE;
  }

  //bother with basic movement sprites if not attacking
  if (!(state&SPACE) && !(state&NOVA)) {
    if (state&INAIR && this->body->GetLinearVelocity().y<0) {
      currentAnimation = rising;
    } else if (state&INAIR && this->body->GetLinearVelocity().y>0) {
      currentAnimation = falling;
    } else if (!(state & (INAIR|LEFT|RIGHT)) || (state&LEFT && state&RIGHT)) {
      currentAnimation = standing;
    } else {
      currentAnimation = walkLeft;
      currentAnimation->start();
    }
  }

  if (state&CATCH) {
    currentAnimation = hanging;
  }
  //decide on the final sprite based on the animation and state
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
  if (state&CATCH) {
    sprite.setRotation(360/8*(hoopdir+2));
  } else {
    sprite.setRotation(this->body->GetAngle() * 180/b2_pi);
  }
  if (!(state&CATCH)) {
  if (state&LEFT) {
    sprite.setScale(-1.f,1.f);
  }
  if (state&RIGHT) {
    sprite.setScale(1.f,1.f);
  }
}

  Window.draw(sprite);



  //FRONT HOOP
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

//WILL EXECUTE EVERY TIME THE COUNTER MEETS THE IF CONDITION
//PUSH 'D' TO DISABLE/ENABLE
void Player::debugPrints() {
  counter++;
  if (counter>60) {
    counter = 0;
    std::cout << "hoop pos x = " << hoop->GetPosition().x<< std::endl;
    std::cout << "player pos x = " << body->GetPosition().x<< std::endl;
    std::cout << ((counter % 2 == 0) ? "PRINTING STATES" : "PRINTING STATES!") << std::endl;
    std::cout << "state int= " << state << std::endl;
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
    std::cout << "JUMP1    = " << ((state&JUMP1) ? "True" : "False") << std::endl;
    std::cout << "JUMP2    = " << ((state&JUMP2) ? "True" : "False") << std::endl;
    std::cout << "C        = " << ((state&C) ? "True" : "False") << std::endl;
    std::cout << "CATCH    = " << ((state&CATCH) ? "True" : "False") << std::endl;
    std::cout << "RETURN   = " << ((state&RETURN) ? "True" : "False") << "\n\n" << std::endl;
  }
}

void Player::handleKeyboard(sf::RenderWindow &Window) {
    sf::Event event;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        state |= LEFT;
    } else {
        state |= LEFT;
        state -= LEFT;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        state |= RIGHT;
    } else {
        state |= RIGHT;
        state -= RIGHT;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        state |= UP;
    } else {
        state |= UP;
        state -= UP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        state |= DOWN;
    } else {
        state |= DOWN;
        state -= DOWN;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !(state&SPACE) && !(state&NOVA) && !(state&THROWN) && !(state&RETURN))
    {
        state |= SPACE;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        state |= C;
    } else {
        if (state & C) {
           state |= JUMPED; // jumped is basically a key released event to allow the second jump
        }
        state |= C;
        state -= C;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !(state&X) && !(state&RETURN)) {
        state |= SPACE;
        state -= SPACE;


        state |= X;
        if (state&THROWN) {
          state |= CATCH;
        } else if (!(state&NOVA) && state&CANNOVA) {
          state |= NOVA;
          state -= CANNOVA;

        }
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        state |= X;
        state -= X;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        debug = !debug;
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        Window.close();
    }
    return;



  return;
  while (Window.pollEvent(event))
  {
    break;
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
                  state |= LEFT;
              }
              if (event.key.code == sf::Keyboard::Right)
              {
                  state |= RIGHT;
              }
              if (event.key.code == sf::Keyboard::Up)
              {
                state |= UP;
              }
              if (event.key.code == sf::Keyboard::Down)
              {
                  state |= DOWN;
              }
              if (event.key.code == sf::Keyboard::Space && !(state&SPACE) && !(state&NOVA) && !(state&THROWN) && !(state&RETURN))
              {

                  state |= SPACE;
              }
              if (event.key.code == sf::Keyboard::X && !(state&X) && !(state&THROWN) && !(state&RETURN))
              {

                  if (state&SPACE) {
                    state -= SPACE;
                  }
                  state |= X;
                  if (!(state&NOVA)) {
                    state |= NOVA;
                  }
              }
              if (event.key.code == sf::Keyboard::C)
              {
                  state |= JUMP1;
              }
              if (event.key.code == sf::Keyboard::Escape)
              {
                  Window.close();
              }
              break;
          case sf::Event::KeyReleased:
              if (event.key.code == sf::Keyboard::Left)
              {
                  state -= LEFT;
              }
              if (event.key.code == sf::Keyboard::Right)
              {
                  state -= RIGHT;
              }
              if (event.key.code == sf::Keyboard::Up && (state&UP))
              {
                state -= UP;
              }
              if (event.key.code == sf::Keyboard::Down && (state&DOWN))
              {
                  state -= DOWN;
              }
              if (event.key.code == sf::Keyboard::Space)
              {
                  // state -= SPACE;
              }
              if (event.key.code == sf::Keyboard::X && (state&X))
              {

                  state -= X;
              }
              break;


          // we don't process other types of events
          default:
              break;
      }
  }
}
