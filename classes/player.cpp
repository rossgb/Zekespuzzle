#include "player.h"
#include "iostream"

static const float SCALE = 30.f;


Player::Player(b2World& World) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody; //this will be a dynamic body
  bodyDef.position.Set(20, 20); //set the starting position
  bodyDef.angle = 0; //set the starting angle

  rising.loadFromFile("MainSprites/basic/jump1.png");
  falling.loadFromFile("MainSprites/basic/jump2.png");
  standing.loadFromFile("MainSprites/basic/idle.png");
  hanging.loadFromFile("MainSprites/basic/coolguyhanging.png");

  std::vector<sf::Texture> anim;

  int i = 0;
  for(i = 0; i<10; i++) {
    std::stringstream sstm;
    sstm << "MainSprites/walk/walkframes_000" << i << "_Frame-" << 10-i << ".png";
    std::string result = sstm.str();
    sf::Texture tex = sf::Texture();
    tex.loadFromFile(result);
    anim.push_back(tex);
  }
   animation walkLeft(anim,6);
  anim.clear();

    for(i = 0; i<10; i++) {
      std::stringstream sstm;
      sstm << "MainSprites/walk/walkframes_000" << i << "_Frame-" << 10-i << ".png";
      std::string result = sstm.str();
      sf::Texture tex = sf::Texture();
      tex.loadFromFile(result);
      anim.push_back(tex);
    }
    animation walkRight(anim,6);
    anim.clear();

    for(i = 0; i<4; i++) {
      std::stringstream sstm;
      sstm << "MainSprites/slash/slashframes_000" << i << "_Frame-" << 4-i << ".png";
      std::string result = sstm.str();
      sf::Texture tex = sf::Texture();
      tex.loadFromFile(result);
      anim.push_back(tex);
    }
    animation slashLeft(anim,6);
    anim.clear();

    for(i = 0; i<4; i++) {
      std::stringstream sstm;
      sstm << "MainSprites/slash/slashframes_000" << i << "_Frame-" << 4-i << ".png";
      std::string result = sstm.str();
      sf::Texture tex = sf::Texture();
      tex.loadFromFile(result);
      anim.push_back(tex);
    }
    animation slashRight(anim,6);
    anim.clear();

    for(i = 0; i<10; i++) {
      std::stringstream sstm;
      sstm << "MainSprites/boom/boomframes_000" << i << "_Frame-" << 10-i << ".png";
      std::string result = sstm.str();
      sf::Texture tex = sf::Texture();
      tex.loadFromFile(result);
      anim.push_back(tex);
    }
    animation nova(anim,4);


  this->body = World.CreateBody(&bodyDef);

  b2PolygonShape Shape;
  Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
  b2FixtureDef FixtureDef;
  FixtureDef.density = 1.f;
  FixtureDef.friction = 0.7f;
  FixtureDef.shape = &Shape;
  this->body->CreateFixture(&FixtureDef);
  this->state = NONE;
}
Player::~Player() {}

void Player::update(sf::RenderWindow &Window) {
  //handle keyboard
  handleKeyboard(Window);

  handleState();

  handlePhysics();

  //get textures from animator and render it to the players position
  handleAnimation(Window);

  //logging
  std::cout<< this->body->GetPosition().t << std::endl;
}


void Player::handlePhysics() {

}

void Player::handleAnimation(sf::RenderWindow &Window) {
    sf::Sprite Sprite;
    //Sprite.setTexture(animator.getTexture());
    Sprite.setOrigin(16.f, 16.f);
    Sprite.setPosition(SCALE * this->body->GetPosition().x, SCALE * this->body->GetPosition().y);
    Sprite.setRotation(this->body->GetAngle() * 180/b2_pi);
    Window.draw(Sprite);
}

void Player::handleState() {
    if (this->state & INAIR == 0 && this->state & UP) {
        this->state += JUMPING;
        this->state += INAIR;
    }

    if (this->state & JUMPING) {
        this->body->ApplyLinearImpulse( b2Vec2(0,50), this->body->GetWorldCenter());
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
