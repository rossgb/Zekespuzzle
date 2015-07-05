#include "animation.h"
#include <sstream>
#include <iostream>

animation::animation(std::string type) {


    std::stringstream sstm;
    sstm << "MainSprites/basic" << type << ".png";
    std::string result = sstm.str();
    sf::Texture* tex = new sf::Texture();
    tex->loadFromFile(result);
    spriteList.push_back(*tex);



  this->frameDelay = 6;
  this->stopped = true;
  this->frameCount = 0;
  this->currentFrame= 0;
  this->animationDirection=0;
  this->totalFrames=1;
}


  animation::animation(std::string type, int end, int delay) {


    int i;
    for(i = 0; i<end; i++) {
      std::stringstream sstm;
      sstm << "MainSprites/" << type << "_000" << i << "_Frame-" << end-i << ".png";
      std::string result = sstm.str();
      sf::Texture* tex = new sf::Texture();
      tex->loadFromFile(result);
      spriteList.push_back(*tex);
    }



    this->frameDelay = delay;
    this->stopped = true;
    this->frameCount = 0;
    this->currentFrame= 0;
    this->animationDirection=-1;
    this->totalFrames=spriteList.size();
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

    sf::Texture* animation::getTexture() {
      std::cout << currentFrame << std::endl;
      return &spriteList[currentFrame];

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
