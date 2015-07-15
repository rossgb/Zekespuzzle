#ifndef PLAYER_H
#define PLAYER_H
#endif

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "animation.h"
#include "entity.h"
#include "blueguy.h"
#include "hoop.h"

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
    RETURN    = 0x1000,
    JUMP1     = 0x2000,
    JUMP2     = 0x4000,
    C         = 0x8000,
    CANNOVA   = 0x10000,
    CATCH     = 0x20000

};



class Player: public Entity {
public:
  Player(b2World& World);
  ~Player();

  void handleCollision(Entity* other, int begin, b2Fixture* thisFix, b2Fixture* otherFix);

  void update(sf::RenderWindow &Window);

  b2Body* body;
  Hoop* hoop;
  b2Fixture* hoopfx;
  b2Fixture* bodyfx;
  b2Filter nocol;
  b2Filter attackcol;
    int i;
  sf::Sprite sprite;
  sf::Sprite hoopbacksp;
  sf::Sprite hoopfrontsp;
  int state;
  int health;
  bool canBeDamaged;
  int facing;

    //debug
    int counter;
    bool debug;
    //debug

private:
  //healthbar STUFF
  sf::RectangleShape healthbar;
  sf::RectangleShape healthoutline;

  int novacounter;
  int dmgcounter;

  int hoopdir;
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
