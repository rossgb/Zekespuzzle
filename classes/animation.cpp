#include "animation.h"


  animation::animation(std::vector<sf::Texture> sprites, int delay) {
    this->frameDelay = delay;
    this->stopped = true;
    this->spriteList = sprites;
    this->frameCount = 0;
    this->currentFrame= 0;
    this->animationDirection=0;
    this->totalFrames=sprites.size();
  }

  void animation::start() {
        if (!stopped) {
            return;
        }

        if (spriteList.size() == 0) {
            return;
        }

        stopped = false;
    }

    void animation::stop() {
        if (spriteList.size() == 0) {
            return;
        }

        stopped = true;
    }

    void animation::restart() {
        if (spriteList.size() == 0) {
            return;
        }

        stopped = false;
        currentFrame = 0;
    }

    void animation::reset() {
        this->stopped = true;
        this->frameCount = 0;
        this->currentFrame = 0;
    }

    sf::Texture animation::getTexture() {
      return spriteList[currentFrame];
    }

    void animation::update() {
      if (!stopped) {
            frameCount++;

            if (frameCount > frameDelay) {
                frameCount = 0;
                currentFrame += animationDirection;

                if (currentFrame > totalFrames - 1) {
                    currentFrame = 0;
                }
                else if (currentFrame < 0) {
                    currentFrame = totalFrames - 1;
                }
            }
        }
    }
