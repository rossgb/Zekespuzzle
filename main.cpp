#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "classes/player.h"
#include "classes/blueguy.h"
#include <iostream>

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 30.f;

/** Create the base for the boxes to land */
void CreateGround(b2World& World, float X, float Y, float width, float height);

/** Create the boxes */
void CreateBox(b2World& World, int MouseX, int MouseY);

int main()
{
    /** Prepare the window */
    sf::RenderWindow Window(sf::VideoMode(1080, 720, 1), "Zeke", sf::Style::Fullscreen);
    // sf::RenderWindow Window(sf::VideoMode(1080, 720, 1), "Zeke");
    Window.setFramerateLimit(60);
		Window.setKeyRepeatEnabled(false);

    sf::View view(sf::FloatRect(0, 0, Window.getSize().x, Window.getSize().y));
    view.setSize(sf::Vector2f(1080*1.5, 720*1.5));



    /** Prepare the world */
    b2Vec2 Gravity(0.f, 19.f);
    b2World World(Gravity, true);
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


    while (Window.isOpen())
    {

        World.Step(1/60.f, 8, 3);
        int darkness = player.body->GetPosition().y/10.f;
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
				player.update(Window);
        blueguy.update(Window);

        std::cout << player.body->GetPosition().x << std::endl;
        std::cout << blueguy.body->GetPosition().x << std::endl;
        std::cout << b2Distance(player.body->GetPosition(),blueguy.body->GetPosition()) << std::endl;


        Window.display();
    }

		return 0;
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
