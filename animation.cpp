#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>

class animation {


  public:
    animation(std::vector<sf::Sprite> sprites, int delay) {
    this->frameDelay = delay;
    this->stopped = true;
    this->spriteList = sprites;
    this->frameCount = 0;
    this->currentFrame= 0;
    this->animationDirection=0;
    this->totalFrames=sprites.size();
  };

  void start() {
        if (!stopped) {
            return;
        }

        if (spriteList.size() == 0) {
            return;
        }

        stopped = false;
    }

    void stop() {
        if (spriteList.size() == 0) {
            return;
        }

        stopped = true;
    }

    void restart() {
        if (spriteList.size() == 0) {
            return;
        }

        stopped = false;
        currentFrame = 0;
    }

    void reset() {
        this->stopped = true;
        this->frameCount = 0;
        this->currentFrame = 0;
    }

    sf::Sprite getSprite() {
      return spriteList[currentFrame];
    }

    void update() {
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

  private:
    int frameCount;
    int frameDelay;
    int currentFrame;
    int animationDirection;
    int totalFrames;

    bool stopped;

    std::vector<sf::Sprite> spriteList;

};
