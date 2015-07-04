#include "player.h"

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
}
Player::~Player() {}

void Player::update() {
  //handle keyboard
  //handle collisions (with box2d)
  //hangle animator calls
}
