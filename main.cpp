#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

//#include "Game.h"
//#include "Games/Pong/Pong.h"
//#include "Games/Snake/SnakeScene.h"
//#include "Games/SpaceInvaders/SpaceInvaders.h"


int main()
{
    /** Prepare the window */
    sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
    Window.setFramerateLimit(60);

    /** Prepare the world */
    b2Vec2 Gravity(0.f, 9.8f);
    b2World World(Gravity, true);

}
