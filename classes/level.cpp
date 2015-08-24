#include "level.h"
#include <fstream>
#include <iostream>
#include <regex>

static const float SCALE = 30.f;



sf::Texture GroundTexture;


Level::Level()
{

}
Level::~Level() {}

void Level::loadFromLv(b2World& World, std::string levelPath)
{
  std::cout << levelPath << std::endl;
  std::string line;
  std::ifstream myfile (levelPath);
  if (myfile.is_open())
  {
    //current level object init
    while ( getline (myfile,line) )
    {
      std::regex groundRegex("(ground),(.*),(.*),(.*),(.*),(.*)");
      std::smatch groundObject;

      if (std::regex_match(line,groundObject,groundRegex))
      {
        std::cout <<  groundObject[5].str() << std::endl;

        CreateGround(World, std::stof(groundObject[2].str()), stof(groundObject[3].str()), stof(groundObject[4].str()), stof(groundObject[5].str()));
        // CreateGround(World, 5, 5, 5, stof(groundObject[5].str()));
        GroundTexture.loadFromFile(groundObject[6]);

        GroundTexture.setRepeated(true);

      }


    }
    std::cout << levelPath << std::endl;

    myfile.close();
  }
  else std::cout << "Unable to open file" << std::endl;
}

void Level::draw(sf::RenderWindow &Window, b2World& World)
{
  for (int i = 0; i < grounds.size(); i++)
  {
    sf::Sprite ground;
    ground.setTexture(GroundTexture);
    ground.setPosition(grounds[i].x,grounds[i].y);
    sf::Rect<int> groundrect2 = sf::Rect<int>(0,0,grounds[i].width,60);
    ground.setTextureRect(groundrect2);
    Window.draw(ground);
  }

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

    //add it to our list of ground structs
    Ground g;
    g.x = X - width/2;
    g.y = Y;
    g.width = width;
    g.height = height;
    grounds.push_back(g);
}
