#include "../include/paint.hpp"

// draws square given mouse position
void drawSquare(sf::RenderWindow& window, const sf::Vector2i& startPos, const sf::Vector2i& endPos) {
    int size = std::min(std::abs(endPos.x - startPos.x), std::abs(endPos.y - startPos.y));
    sf::RectangleShape square(sf::Vector2f(static_cast<float>(size), static_cast<float>(size)));
    square.setPosition(static_cast<float>(startPos.x), static_cast<float>(startPos.y));

    window.draw(square);
}

void drawPixel(sf::RenderWindow& window, const sf::Vector2i& mousePosition) {
        // creates a pixel of size 2 at the mouse position
        sf::CircleShape pixel(2); 
        pixel.setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        // draws the pixel
        window.draw(pixel);
}

// handles: pressing the X button on the window, right click press and right click release
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawing, sf::Vector2i& startPos) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // if right click is pressed
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            isDrawing = true;
            startPos = sf::Mouse::getPosition(window);
        }

        // if right click is released
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
            isDrawing = false;
        }
    }
}

// main drawing loop, handles all artsy stuff and ties the other functions together
void paintLoop(sf::RenderWindow& window) {
    bool isDrawing = false;  
    sf::Vector2i startPos;  
        
    while (window.isOpen()) {
        handleDrawingEvents(window, isDrawing, startPos);
        // if in drawing mode (holding down right mouse)
        if (isDrawing) {
            sf::Vector2i currentPos = sf::Mouse::getPosition(window); 
            drawSquare(window, startPos, currentPos);
        }

        // saves the position of the mouse
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        drawPixel(window, mousePos);

        // displays the changes
        window.display();
    }
}