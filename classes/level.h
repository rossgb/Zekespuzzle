#ifndef LEVEL_H
#define LEVEL_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <string>

class Level
{
public:
  Level();
  ~Level();

  void loadFromJson(b2World& World, std::string levelPath);
  void draw(sf::RenderWindow &Window);

private:
  void CreateGround(b2World& World, float X, float Y, float width, float height);
};

#endif
