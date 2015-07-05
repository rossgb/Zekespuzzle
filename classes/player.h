#ifndef PLAYER_H
#define PLAYER_H
#endif

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "animation.h"

enum StateMask {
    NONE      = 0x0,
    LEFT      = 0x1,
    RIGHT     = 0x2,
    UP        = 0x4,
    DOWN      = 0x8,
    SPACE     = 0x10,
    INAIR     = 0x20,
    ATTACKING = 0x40,
    JUMPED    = 0x80,
    NOVA      = 0x100,
    X         = 0x200,
    POSED     = 0x400,
    THROWN    = 0x800,
    RETURN    = 0x1000

};



class Player {
public:
  Player(b2World& World);
  ~Player();

  void update(sf::RenderWindow &Window);

  b2Body* body;
  b2Body* hoop;
  int i;
  sf::Sprite sprite;
  sf::Sprite hoopbacksp;
  sf::Sprite hoopfrontsp;
  int state;

    //debug
    int counter;
    bool debug;
    void debugPrints();
    //debug

private:
  int novacounter;
  float lastx;
  float lasty;
  b2Vec2 hooporigin;

  sf::Texture hoopback;
  sf::Texture hoopfront;
  sf::Texture hoopflat;

  animation* rising;
  animation* falling;
  animation* standing;
  animation* hanging;

  animation* walkLeft;
  animation* walkRight;
  animation* slashLeft;
  animation* slashRight;
  animation* nova;


  void handleKeyboard(sf::RenderWindow &Window);  //sets the state based on keyboard input
  void handleState();                             //resolves conflicting states
  void handlePhysics();                           //handles movement based on states
  void handleAnimation(sf::RenderWindow &Window); //gets the texture from the animator based on the state
  animation* currentAnimation;

  int* numFootContacts;

};
