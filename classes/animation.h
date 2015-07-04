#ifndef ANIMATION_H
#define  ANIMATION_H
#endif

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>


class animation {
public:
  animation();
  animation(std::vector<sf::Texture> sprites, int delay);

  void start();

  void stop();

  void restart();

  void reset();

  sf::Texture getTexture();

  void update();


private:
  int frameCount;
  int frameDelay;
  int currentFrame;
  int animationDirection;
  int totalFrames;

  bool stopped;

  std::vector<sf::Texture> spriteList;
};
