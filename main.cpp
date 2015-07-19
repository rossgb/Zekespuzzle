#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "classes/player.h"
#include "classes/blueguy.h"
#include <iostream>
#include "classes/listener.h"
#include "classes/emitter.h"

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 30.f;

/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y, float width, float height);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);

void debugPrints(sf::RenderWindow &Window, sf::View view);

void handleInput(b2World& World, Player player, sf::RenderWindow &Window);

int counter;

int main()
{
    /** Prepare the window */
    // sf::RenderWindow Window(sf::VideoMode(1080, 720, 1), "Zeke", sf::Style::Fullscreen);
    sf::RenderWindow Window(sf::VideoMode(1080, 720, 1), "Zeke");
    Window.setFramerateLimit(60);
		Window.setKeyRepeatEnabled(false);

    sf::View view(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));
    view.setSize(sf::Vector2f(1080*1.5, 720*1.5));

    ContactListener listener;

    /** Prepare the world */
    b2Vec2 Gravity(0.f, 19.f);
    b2World World(Gravity, true);

    World.SetContactListener(&listener);
    CreateGround(World, 600.f, 800.f, 1600.f, 16.f);
    CreateGround(World, 200.f, 250.f, 120.f, 20.f);
    CreateGround(World, 300.f, 500.f, 120.f, 20.f);
    /** Prepare textures */

    sf::Texture GroundTexture;
    GroundTexture.loadFromFile("MainSprites/ground/groundtiles_0001_Layer-6.png");
    GroundTexture.setRepeated(true);
    sf::Texture GroundTexture2;
    GroundTexture2.loadFromFile("MainSprites/ground/groundtiles_0006_Layer-9.png");
    GroundTexture2.setRepeated(true);


    sf::Texture BoxTexture;

		Player player(World);

    BlueGuy blueguy(World,player.body);

    int mystupidchecker = 1;

    emitter particles = emitter(1000);
    while (Window.isOpen())
    {

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

        sf::Sprite ground;
        ground.setTexture(GroundTexture);
        ground.setPosition(140.f,250.f);
        sf::Rect<int> groundrect = sf::Rect<int>(0,0,120,60);
        ground.setTextureRect(groundrect);
        Window.draw(ground);

        ground.setTexture(GroundTexture);
        ground.setPosition(0.f,800.f);
        sf::Rect<int> groundrect2 = sf::Rect<int>(0,0,1600,60);
        ground.setTextureRect(groundrect2);
        Window.draw(ground);

        ground.setTexture(GroundTexture);
        ground.setPosition(240.f,500.f);
        ground.setTextureRect(groundrect);
        Window.draw(ground);


        ground.setTexture(GroundTexture2);
        ground.setPosition(0.f,860.f);
        ground.setTextureRect(groundrect2);
        Window.draw(ground);

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
        debugPrints(Window, view);
        handleInput(World, player, Window);
        Window.display();
    }

		return 0;
}

void handleInput(b2World& World, Player player, sf::RenderWindow &Window) {
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
void debugPrints(sf::RenderWindow &Window, sf::View view) {

  sf::Font font;
  font.loadFromFile("etc/FreeMono.ttf");
  sf::Text text;
  text.setFont(font);
  text.setColor(sf::Color::Black);
  text.setString("test");
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


void CreateBox(b2World& World, int MouseX, int MouseY)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(MouseX/SCALE, MouseY/SCALE);
    BodyDef.type = b2_dynamicBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f/2)/SCALE, (32.f/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.7f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World& World, float X, float Y, float width, float height)
{
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(X/SCALE, Y/SCALE);
    BodyDef.type = b2_staticBody;
    b2Body* Body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((width/2)/SCALE, (height/2)/SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 0.f;
    FixtureDef.shape = &Shape;
    Body->CreateFixture(&FixtureDef);
}
