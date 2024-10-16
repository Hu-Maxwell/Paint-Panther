#include "../include/paint.hpp"

// draws a pixel at the given mouse pos
void drawPixel(sf::RenderTexture& canvas, const sf::Vector2f& mousePosition) {
    sf::CircleShape pixel(2);
    pixel.setPosition(mousePosition);
    pixel.setFillColor(sf::Color::Black);
    canvas.draw(pixel);
}

// main drawing loop
void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas) {

    sf::Vector2i startPos;

    int currentTool = 0; // 0 = nothing, 1 = pen, 2 = rectangle

    sf::FloatRect penButtonBounds;
    sf::FloatRect rectangleButtonBounds;

    bool isDrawingrectangle = false;
    bool isDrawingPixel = false;

    while (window.isOpen()) {
        handleDrawingEvents(window, isDrawingrectangle, isDrawingPixel, startPos, currentTool, penButtonBounds, rectangleButtonBounds);

        // draw rectangle
        if (isDrawingrectangle) {
            drawRectangle(canvas, startPos, sf::Mouse::getPosition(window));
        }

        // draw pixel
        if (isDrawingPixel) {
            sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            drawPixel(canvas, mouseCoords);
        }

        // draw canvas
        sf::Sprite canvasSprite(canvas.getTexture());
        window.draw(canvasSprite);
        canvas.display();  

        // draw UI
        drawUI(window, penButtonBounds, rectangleButtonBounds);

        window.display();
    }
}