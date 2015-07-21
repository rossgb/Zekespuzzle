#include "level.h"
#include <fstream>
#include <iostream>


static const float SCALE = 30.f;



sf::Texture GroundTexture;


Level::Level()
{

}
Level::~Level() {}

void Level::loadFromJson(b2World& World, std::string levelPath)
{
  std::cout << levelPath << std::endl;
  std::string line;
  std::ifstream myfile (levelPath);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      std::cout << line << '\n';
    }
    myfile.close();
  }
  else std::cout << "Unable to open file" << std::endl;

  //@Ross, put this in the above loop
  CreateGround(World, 600.f, 800.f, 1600.f, 16.f);
  GroundTexture.loadFromFile("MainSprites/ground/groundtiles_0001_Layer-6.png");
  GroundTexture.setRepeated(true);
}

void Level::draw(sf::RenderWindow &Window)
{
  sf::Sprite ground;
  ground.setTexture(GroundTexture);
  ground.setPosition(0.f,800.f);
  sf::Rect<int> groundrect2 = sf::Rect<int>(0,0,1600,60);
  ground.setTextureRect(groundrect2);
  Window.draw(ground);
}

void Level::CreateGround(b2World& World, float X, float Y, float width, float height)
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
