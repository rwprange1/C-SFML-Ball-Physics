#include "Display.h"


/**
 * The constructor for this
 */
Display::Display(){
    this->videoMode = sf::VideoMode({1000, 800});
    this->window = sf::RenderWindow(this->videoMode, "Game", sf::Style::Titlebar | sf::Style::Close);
    this->window.setFramerateLimit(0);
    this->buildObjects();
    this->mouseHeld = false;

    
}

/**
 * This function will check to see if the window is still open
 * that is the game is still running
 * returns a bool on if the game is running still
 */
bool Display::isWinOpen(){
    return this->window.isOpen();
}


/**
 * Draws all of the objects to the screen
 * @returns void
 */
void Display::render(){
    this->window.clear();


   

    for (auto &e: this->objects){
        this->window.draw(e.shape);
    }
   


    this->window.display();
}

/**
 * This method will handle the user input and update the object accordingly
 * @returns void
 */
void Display::update(){
    this->pollEvents();
    this->updateMouse();
    this->updateObjects();

}
 

/** 
 * Given user input handle it
 * @returns void
 */
void Display::pollEvents(){
    //event polling
    while(const std::optional event = this->window.pollEvent()){
        
        if (event ->is<sf::Event::Closed>()){
            window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            const sf::Keyboard::Scancode key = keyPressed->scancode;

            switch(key){
                case sf::Keyboard::Scancode::Escape:
                    std::cout << "Escape Was Hit" << std::endl;
                    window.close();
                    break;
                case sf::Keyboard::Scancode::Space:
                    std::cout << "Space Was Hit" << std::endl;  
                    break;  
                default:
                    std::cout << "Unkown Key Bind\n";


            }

            
        }
    }
}


/**
 * Spawn in 3-6 circles which the user will be able to move 
 * @retuns void
*/
void Display::buildObjects(){
    

    int numObjects = 10 + (rand() % 6);
    //int numObjects = 2;
    for (int i = 0; i < numObjects; i++) {
        sf::CircleShape circle; 

        circle.setRadius(40.f);
  
        sf::Vector2f pos = sf::Vector2f(
            rand() % static_cast<int>(this->window.getSize().x - circle.getRadius()),
            rand() % static_cast<int>(this->window.getSize().y - circle.getRadius())
        );
        circle.setPosition(pos);

     
        
        while (!this->validatePos(circle)){
            sf::Vector2f pos = sf::Vector2f(
            rand() % static_cast<int>(this->window.getSize().x - circle.getRadius()),
            rand() % static_cast<int>(this->window.getSize().y - circle.getRadius())
            );
            circle.setPosition(pos);

        }
        

        circle.setFillColor(sf::Color::Red);

        this->objects.push_back(
            Object{
                circle,
                i
            }

        );

        

        
    }
    std::cout << "Generated " + std::to_string(numObjects) << " balls :)\n";
}

/**
 * Checks to see if a ball is on or in a wall
 */
bool Display::isCollisionWithWall(sf::CircleShape c1){
    int posX = c1.getPosition().x;
    int posY = c1.getPosition().y;
    return  
        (posX < 0 || posX > (this->window.getSize().x -  2 * c1.getRadius())) 
        || 
        (posY < 0 || posY > (this->window.getSize().y - 2 * c1.getRadius()));
}


/**
 * On spawn of balls make sure none are in invalid positions
 * @returns bol, false if not valid, true otherwise
 */
bool Display::validatePos(sf::CircleShape c1) {
    for (auto &c2: this->objects){
        if (&c1 != &c2.shape && this->isCollisionWithBall(c1,c2.shape)) {
            return false;
        }
        if (this->isCollisionWithWall(c1)){
            return false;
        }
    }

    return true;

}

/**
 * Checks to see if two balls are colliding
 */
bool Display::isCollisionWithBall(sf::CircleShape c1, sf::CircleShape c2){
    sf::Vector2f c1Point = c1.getPosition();
    sf::Vector2f c2Point = c2.getPosition();
    
    float a = std::abs( c1Point.x - c2Point.x);
    float b = std::abs( c1Point.y - c2Point.y);
    float diff = a*a + b*b;

    return diff <= (c1.getRadius() + c2.getRadius()) * (c1.getRadius() + c2.getRadius());

}

/**
 * If the balls are colliding have some type of collision and resolve the collison
 * @returns void
 */
void Display::updateObjects(){


    

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        
        
        
        this->currentlySelectedObj = nullptr;

        for (auto &e: this->objects){
            if (this->isPointInCircle(e.shape)){
                this->currentlySelectedObj = &e;
                break;
            }
        }
        if(this->mouseHeld && this->currentlySelectedObj != nullptr){
            sf::Vector2f prev = this->currentlySelectedObj->shape.getPosition();

            this->currentlySelectedObj->shape.setPosition(this->mousePosView);

            if(this->isCollisionWithWall(this->currentlySelectedObj->shape)){
                this->currentlySelectedObj->shape.setPosition(prev);
                std::cout << "Wall mf \n";
            }
        }else{
            this->mouseHeld = true;
        }

       

    }else{
        this->currentlySelectedObj = nullptr;
        this->mouseHeld = false;
    }   

    


    for (auto &c1: this->objects){
   
        for (auto &c2: this -> objects) {

            if (c1.id != c2.id && this->isCollisionWithBall(c1.shape, c2.shape)){
                    
                    sf::Vector2f c1Point = c1.shape.getPosition();
                    sf::Vector2f c2Point = c2.shape.getPosition();
                    
                    float a = std::abs( c1Point.x - c2Point.x);
                    float b = std::abs( c1Point.y - c2Point.y);
                    float diff = std::sqrt(a*a + b*b);
                    float overlap = std::fabs((diff - c1.shape.getRadius() - c2.shape.getRadius()) * .5f);
                    
                    
                    float c1X = c1.shape.getPosition().x;
                    float c1Y = c1.shape.getPosition().y;
                    float c2X = c2.shape.getPosition().x;
                    float c2Y = c2.shape.getPosition().y;
                    
                    float dX1 = overlap * (c1X - c2X ) / diff;
                    float dY1 = overlap * (c1Y - c2Y ) / diff; 



                    float dX2 = overlap * (c2X - c1X ) / diff;
                    float dY2 = overlap * (c2Y - c1Y ) / diff; 

                    c1.shape.move(sf::Vector2f(dX1, dY1));
                    c2.shape.move(sf::Vector2f(dX2, dY2));
                    
                    if (this->isCollisionWithWall(c1.shape)){
                        c1.shape.setPosition(c1Point);
                    }
                     if (this->isCollisionWithWall(c2.shape)){
                        c2.shape.setPosition(c2Point);
                    }


                  
            }
        }
    }

}

/**
 * Sets the mouse pos field to the relative positon if the mouse on the screen
 */
void Display::updateMouse(){
    this->mousePosWindow = sf::Mouse::getPosition(this->window);
    //mapped to our view
    this->mousePosView = this->window.mapPixelToCoords(this->mousePosWindow);
}

/**
 * A helper method which calculates whether we have clicked in the shape or not
 * @returns true if the click is within
 */
bool Display::isPointInCircle(sf::CircleShape c1){
    float distance = hypot((this->mousePosView.x - c1.getPosition().x), (this->mousePosView.y - c1.getPosition().y) ); //Checks the distance between the mouse and each circle's center
    return distance <= c1.getRadius();
}
