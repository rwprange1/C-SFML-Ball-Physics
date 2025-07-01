#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


struct Object {
    sf::CircleShape shape;
    int id;
    float vx, vy;

};



class Display {

    private:
        sf::RenderWindow window;
        sf::VideoMode videoMode;
        sf::Vector2i mousePosWindow; // mouse pos rel to window
        sf::Vector2f mousePosView; 

        Object *currentlySelectedObj = nullptr;

        std::vector<Object> objects;


        bool mouseHeld;


        //Functions
        void pollEvents();
        void buildObjects();
        bool validatePos(sf::CircleShape);
        bool isCollisionWithWall(sf::CircleShape);
        bool isCollisionWithBall(sf::CircleShape, sf::CircleShape);
        void updateObjects();
        void updateMouse();
        bool isPointInCircle(sf::CircleShape);

  

    public:
        Display();
   


        //Functions
        void update();
        void render();
        bool isWinOpen();

        
};