#include "blueguy.h"
#include "iostream"
#include <stdlib.h>
#include <iostream>

static const float SCALE = 30.f;

BlueGuy::BlueGuy(b2World& World, b2Body* playerbod) {
  rising = new animation("bluejump");
  standing = new animation("blueguy/blue", 8,20);
  walkLeft = new animation("bluewalk/bluewalk", 4, 15);
  die = new animation("bluedeath/bluedeath", 6, 8);

  player = playerbod;

  currentAnimation = standing;
  currentAnimation->start();

  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody; //this will be a dynamic body
  bodyDef.position.Set(20, 5); //set the starting position
  bodyDef.angle = 0; //set the starting angle
  bodyDef.fixedRotation = true;



  this->body = World.CreateBody(&bodyDef);
  body->SetUserData(this);

  b2PolygonShape Shape;
  Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
  b2FixtureDef FixtureDef;
  FixtureDef.filter.categoryBits = 4;
  FixtureDef.filter.maskBits = 1;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 5.f;
  FixtureDef.shape = &Shape;
  this->body->CreateFixture(&FixtureDef);

  b2FixtureDef sensor;
  sensor.isSensor = true;
  sensor.shape = &Shape;
  sensor.friction = 0.f;
  this->body->CreateFixture(&sensor);


  state = BNONE;
  health = 3;

}
BlueGuy::~BlueGuy() {}

void BlueGuy::update(sf::RenderWindow &Window) {
  //handle keyboard
  if (!(state&BDEAD)) {
  currentAnimation->update();

  handleState();

  handlePhysics();


  //get textures from animator and render it to the players position
  handleAnimation(Window);
}

}

void BlueGuy::handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix) {

}


void BlueGuy::handleState() {
  if (b2Distance(player->GetPosition(),body->GetPosition())<8) {
    // std::cout << "REMOVING BNONE" <<std::endl;

    if (player->GetPosition().x>body->GetPosition().x) {
      state |= BRIGHT;

      state |= BLEFT;
      state -= BLEFT;
      state |= BNONE;
      state -= BNONE;
    } else {
      state |= BLEFT;
      state |= BRIGHT;
      state -= BRIGHT;
      state |= BNONE;
      state -= BNONE;
    }
  } else {
    // std::cout << "ADDING BNONE" <<std::endl;

    state |= BNONE;
    // state |= BRIGHT;
    // state -= BRIGHT;
    // state |= BLEFT;
    // state -= BLEFT;
  }
  if (health <= 0) {
    state |= BDIE;
  }
  // std::cout << "bnone state = " << (state&BNONE ? "true" : "false") <<std::endl;
  // std::cout << "Right state = " << (state&BRIGHT ? "true" : "false") <<std::endl;
  // std::cout << "left state = " << (state&BLEFT ? "true" : "false") <<std::endl;

}

void BlueGuy::handlePhysics() {

    //if the velocity hits zero and the character is in the air, ground and add a jump
    if (this->body->GetLinearVelocity().y == 0) {
        state |= BINAIR;
        state -= BINAIR;

        //reset all jumps
        state |= BUP;
        state -= BUP;
    }

    //if y velocity is there then character is in the air
    if (abs(this->body->GetLinearVelocity().y) > 0.1 && !(state & BINAIR)) {
        state |= BINAIR;
    }

    //walking force
    int force = 40;
    //reduce control in air/while attacking
    if (state & BINAIR) {
      force /=4;
    }
    if (state & BLEFT && !(state&BNONE)) {
      this->body->ApplyForce( b2Vec2(-force,0), this->body->GetWorldCenter() );
    }
    if (state&BRIGHT && !(state&BNONE)) {

      // std::cout << "WHAT" <<std::endl;

      this->body->ApplyForce( b2Vec2(force,0), this->body->GetWorldCenter() );
    }

    // if (state&BNONE) {
    //   body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x/10,body->GetLinearVelocity().y/10));
    // }

    //if not nova
      //if jump1 is false
        //first jump
      //if jump1 is true and jump2 is false
        //second jump
    if (state&BUP) {
        if (!(state & BJUMP)) {
            state |= BJUMP;
            this->body->SetLinearVelocity(b2Vec2(this->body->GetLinearVelocity().x,-0.1));
            this->body->ApplyLinearImpulse( b2Vec2(0,-16), this->body->GetWorldCenter());
        } }

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
    if (this->body->GetLinearVelocity().x>6) {
      this->body->SetLinearVelocity(b2Vec2(6,this->body->GetLinearVelocity().y));
    }

    if (this->body->GetLinearVelocity().x<-6) {
      this->body->SetLinearVelocity(b2Vec2(-6,this->body->GetLinearVelocity().y));
    }

    if (state&BDIE) {
      body->SetLinearVelocity(b2Vec2(0,0));
    }

}

void BlueGuy::handleAnimation(sf::RenderWindow &Window) {

  if (state&BLEFT || state&BRIGHT) {
    currentAnimation = walkLeft;
  }

  if (state&BJUMP) {
    currentAnimation = rising;
  }

  if (state&BNONE) {
    currentAnimation = standing;
  }

  if (state&BDIE) {
    currentAnimation = die;
    if (currentAnimation->currentFrame == 1 && currentAnimation->frameCount == 6) {
      currentAnimation->frameCount++;
      currentAnimation->stop();
      state |= BDEAD;
      Orb* o = new Orb(body, (int)body->GetPosition().x, (int)body->GetPosition().y);
    }
  }



  currentAnimation->start();

  //decide on the final sprite based on the animation and state
  sprite.setTexture(*currentAnimation->getTexture());

  sprite.setOrigin(30.f, 90.f);
  sprite.setPosition(SCALE * this->body->GetPosition().x, SCALE * this->body->GetPosition().y);

    sprite.setRotation(this->body->GetAngle() * 180/b2_pi);
  if (state&BLEFT) {
    sprite.setScale(-1.f,1.f);
  }
  if (state&BRIGHT) {
    sprite.setScale(1.f,1.f);
  }

  if (!(state&BDEAD)) {
    Window.draw(sprite);
  }



}
