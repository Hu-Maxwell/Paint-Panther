#include "../include/paint.hpp"

enum Tool {
    Pen,
    Rectangle
};

// draws square given mouse position
void drawSquare(sf::RenderTexture& canvas, sf::Vector2i& startPos, sf::Vector2i endPos, sf::Vector2u windowSize) {
    startPos.y = windowSize.y - startPos.y;
    endPos.y = windowSize.y - endPos.y;

    // calculate width and height given start and end mouse positions
    float width = endPos.x - startPos.x;
    float height = endPos.y - startPos.y;

    sf::RectangleShape rectangle(sf::Vector2f(std::abs(width), std::abs(height)));

    // find the top left corner of rectangle
    rectangle.setPosition(
        width < 0 ? endPos.x : startPos.x,
        height < 0 ? endPos.y : startPos.y
    );

    rectangle.setFillColor(sf::Color::Black);
    canvas.draw(rectangle);

}

void drawPixel(sf::RenderTexture& canvas, const sf::Vector2f& mousePosition) {
        // creates a pixel of size 2 at the mouse position
        sf::CircleShape pixel(2); 
        pixel.setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
        pixel.setFillColor(sf::Color::Black);

        // draws the pixel
        canvas.draw(pixel);
}

// handles: pressing the X button on the window, right click press and right click release
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingSquare, bool& isDrawingPixel, sf::Vector2i& startPos, Tool& currentTool, const sf::FloatRect& penButtonBounds, const sf::FloatRect& rectangleButtonBounds) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // Check if the Pen button was clicked
            if (penButtonBounds.contains(mousePos)) {
                currentTool = Tool::Pen;
            }

            // Check if the Rectangle button was clicked
            if (rectangleButtonBounds.contains(mousePos)) {
                currentTool = Tool::Rectangle;
            }
        }
    }
}

void drawUI(sf::RenderWindow& window, sf::FloatRect& penButtonBounds, sf::FloatRect& rectangleButtonBounds) {
    // Set up toolbar UI element
    sf::RectangleShape toolbar(sf::Vector2f(window.getSize().x, 50));
    toolbar.setFillColor(sf::Color(200, 200, 200));

    // Set up Pen button
    sf::RectangleShape penButton(sf::Vector2f(100, 30));
    penButton.setPosition(10, 10);
    penButton.setFillColor(sf::Color(150, 150, 150));
    penButtonBounds = penButton.getGlobalBounds(); // Update the bounds to pass back

    // Set up Rectangle button
    sf::RectangleShape rectangleButton(sf::Vector2f(100, 30));
    rectangleButton.setPosition(120, 10);
    rectangleButton.setFillColor(sf::Color(150, 150, 150));
    rectangleButtonBounds = rectangleButton.getGlobalBounds(); // Update the bounds to pass back

    // Draw the toolbar and buttons
    window.draw(toolbar);
    window.draw(penButton);
    window.draw(rectangleButton);
}


// main drawing loop, handles all artsy stuff and ties the other functions together
void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas) {
    bool isDrawingSquare = false;  
    bool isDrawingPixel = false; 

    sf::Vector2i startPos;  

    Tool currentTool = Tool::Pen;

    sf::FloatRect penButtonBounds;
    sf::FloatRect rectangleButtonBounds;
        
    while (window.isOpen()) {
        handleDrawingEvents(window, isDrawingSquare, isDrawingPixel, startPos, currentTool, penButtonBounds, rectangleButtonBounds);

        sf::Vector2u windowSize = window.getSize();

        // draws square

        if (isDrawingSquare) {
            // sf::Vector2f startCoords = window.mapPixelToCoords(startPos);
            // sf::Vector2f currentCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // for some reason, when drawing on a texture, the y axis gets flipped, so we manually un-flip it

            drawSquare(canvas, startPos, sf::Mouse::getPosition(window), windowSize);
        }

        // draws pixel
        sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        mouseCoords.y = windowSize.y - mouseCoords.y; 
        drawPixel(canvas, mouseCoords);

        // draw canvas
        sf::Sprite canvasSprite(canvas.getTexture());
        window.draw(canvasSprite);

        // draw UI
        drawUI(window, penButtonBounds, rectangleButtonBounds);

        // displays the changes
        window.display();
    }
}