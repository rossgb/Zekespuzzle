#ifndef ANIMATION_H
#define  ANIMATION_H


#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <iostream>


class animation {
public:
  animation();
  animation(std::string type);

  animation(std::string type, int i, int delay);

  void start();

  void stop();

  void restart();

  void reset();

  sf::Texture* getTexture();

  void update();

std::vector<sf::Texture> spriteList;


  int frameCount;
  int frameDelay;
  int currentFrame;
  int animationDirection;
  int totalFrames;

  bool stopped;


};
#endif
