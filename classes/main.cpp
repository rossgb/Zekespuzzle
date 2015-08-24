#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <sstream>
#include "player.h"
#include "blueguy.h"
#include "listener.h"
#include "emitter.h"
#include "level.h"

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 30.f;

/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y, float width, float height);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);

void debugPrints(sf::RenderWindow &Window, sf::View view, float fps);

void handleInput(b2World& World, Player &player, sf::RenderWindow &Window);

int counter;

sf::Font font;

int main()
{
    /** Prepare the window */
    // sf::RenderWindow Window(sf::VideoMode(1080, 720, 1), "Zeke", sf::Style::Fullscreen);
    sf::RenderWindow Window(sf::VideoMode(640, 480, 1), "Zeke");
    Window.setFramerateLimit(60);
		//Window.setKeyRepeatEnabled(false);

    sf::View view(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));
    view.setSize(sf::Vector2f(1080*1.5, 720*1.5));

    ContactListener listener;

    /** Prepare the world */
    b2Vec2 Gravity(0.f, 19.f);
    b2World World(Gravity, true);

    World.SetContactListener(&listener);

    /** Load Level */
    Level level;
    level.loadFromLv(World, "./levels/level1.lv");

    /** Load Fonts */
    font.loadFromFile("etc/FreeMono.ttf");


    sf::Texture BoxTexture;

		Player player(World);

    BlueGuy blueguy(World,player.body);

    int mystupidchecker = 1;

    emitter particles = emitter(1000);

    sf::Clock clock;

    while (Window.isOpen())
    {
        sf::Time timeSinceLastFrame = clock.getElapsedTime();
        clock.restart();
        float fps =  1 / (timeSinceLastFrame.asSeconds());

        World.Step(1/60.f, 8, 3);
        int darkness = player.body->GetPosition().y*3.f;
        int r = 101-darkness;
        int g = 191-darkness;
        int b = 214-darkness;
        r = (r<=0)?0:r;
        g = (g<=0)?0:g;
        b = (b<=0)?0:b;
        Window.clear(sf::Color(r,g,b,255));

        view.setCenter(player.body->GetPosition().x * SCALE, player.body->GetPosition().y * SCALE);
        Window.setView(view);

        level.draw(Window, World);

        for (b2Body* b = World.GetBodyList(); b; b = b->GetNext()) {
          Entity* cur;
          if (cur = (Entity*)b->GetUserData()) {
            cur->update(Window);

            if (BlueGuy* bg = dynamic_cast<BlueGuy*>(cur)) {
              if (bg->state&BDEAD) {
              World.DestroyBody(bg->body);
            }
            }
          }
        }

        counter++;


        particles.update();
        particles.setEmitter(player.body->GetPosition().x*30,player.body->GetPosition().y*30);
        Window.draw(particles);


        // std::cout << player.body->GetPosition().x << std::endl;
        // std::cout << blueguy.body->GetPosition().x << std::endl;
        // std::cout << b2Distance(player.body->GetPosition(),blueguy.body->GetPosition()) << std::endl;
        debugPrints(Window, view, fps);
        handleInput(World, player, Window);
        Window.display();
    }

		return 0;
}

void handleInput(b2World& World, Player &player, sf::RenderWindow &Window) {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
      if (counter>60) {
      BlueGuy* bg = new BlueGuy(World, player.body, sf::Mouse::getPosition(Window).x, sf::Mouse::getPosition(Window).y);
      counter = 0;
    }
  }
}
//WILL EXECUTE EVERY TIME THE COUNTER MEETS THE IF CONDITION
//PUSH 'D' TO DISABLE/ENABLE
void debugPrints(sf::RenderWindow &Window, sf::View view, float fps) {

  sf::Text text;
  text.setFont(font);
  text.setColor(sf::Color::Magenta);
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
  ss << "fps: " << fps;
  text.setString(ss.str());
  text.setPosition(view.getCenter().x-view.getSize().x/2,view.getCenter().y-view.getSize().y/2);
  Window.draw(text);
  // if (counter>60) {
  //   counter = 0;
  //   std::cout << "hoop pos x = " << hoop->GetPosition().x<< std::endl;
  //   std::cout << "player pos x = " << body->GetPosition().x<< std::endl;
  //   std::cout << ((counter % 2 == 0) ? "PRINTING STATES" : "PRINTING STATES!") << std::endl;
  //   std::cout << "state int= " << state << std::endl;
  //   std::cout << "LEFT     = " << ((state&LEFT) ? "True" : "False") << std::endl;
  //   std::cout << "RIGHT    = " << ((state&RIGHT) ? "True" : "False") << std::endl;
  //   std::cout << "UP       = " << ((state&UP) ? "True" : "False") << std::endl;
  //   std::cout << "DOWN     = " << ((state&DOWN) ? "True" : "False") << std::endl;
  //   std::cout << "SPACE    = " << ((state&SPACE) ? "True" : "False") << std::endl;
  //   std::cout << "INAIR    = " << ((state&INAIR) ? "True" : "False") << std::endl;
  //   std::cout << "ATTACKING= " << ((state&ATTACKING) ? "True" : "False") << std::endl;
  //   std::cout << "JUMPED   = " << ((state&JUMPED) ? "True" : "False") << std::endl;
  //   std::cout << "NOVA     = " << ((state&NOVA) ? "True" : "False") << std::endl;
  //   std::cout << "X        = " << ((state&X) ? "True" : "False") << std::endl;
  //   std::cout << "POSED    = " << ((state&POSED) ? "True" : "False") << std::endl;
  //   std::cout << "THROWN   = " << ((state&THROWN) ? "True" : "False") << std::endl;
  //   std::cout << "JUMP1    = " << ((state&JUMP1) ? "True" : "False") << std::endl;
  //   std::cout << "JUMP2    = " << ((state&JUMP2) ? "True" : "False") << std::endl;
  //   std::cout << "C        = " << ((state&C) ? "True" : "False") << std::endl;
  //   std::cout << "CATCH    = " << ((state&CATCH) ? "True" : "False") << std::endl;
  //   std::cout << "RETURN   = " << ((state&RETURN) ? "True" : "False") << "\n\n" << std::endl;
  // }
}
