#ifndef LEVEL_H
#define LEVEL_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum GroudType {
    LEFTPLAT      = 0x1,
    RIGHTPLAT      = 0x2,
    CENTER     = 0x4,
    LEFTDIRT        = 0x8,
    RIGHTDIRT      = 0x10,
    CENTERDIRT     = 0x20,
    LEFTFLOAT     = 0x40,
    RIGHTFLOAT = 0x80,
    CENTERFLOAT    = 0x100

};

struct Ground
{
  float x;
  float y;
  float width;
  float height;
};

class Level
{
public:
  Level();
  ~Level();

  void loadFromLv(b2World& World, std::string levelPath);
  void draw(sf::RenderWindow &Window, b2World& World);

private:
  void CreateGround(b2World& World, float X, float Y, float width, float height);

  std::vector<Ground> grounds;
};

#endif
