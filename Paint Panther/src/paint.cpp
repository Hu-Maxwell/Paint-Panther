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

    int currentTool = 0; // 0 = nothing, 1 = pen, 2 = rectangle, 3 = circle

    sf::FloatRect penButtonBounds;
    sf::FloatRect rectangleButtonBounds;
    sf::FloatRect circleButtonBounds;
    sf::FloatRect undoButtonBounds;
    sf::FloatRect redoButtonBounds;

    bool isDrawingRectangle = false;
    bool isDrawingPixel = false;
    bool isDrawingCircle = false;

    while (window.isOpen()) {
        handleDrawingEvents(window, isDrawingRectangle, isDrawingPixel, isDrawingCircle, startPos, currentTool, penButtonBounds, rectangleButtonBounds, circleButtonBounds, undoButtonBounds, redoButtonBounds);

        // draw rectangle
        if (isDrawingRectangle) {
            drawRectangle(canvas, startPos, sf::Mouse::getPosition(window));
        }

        // draw circle
        if (isDrawingCircle) {
            drawCircle(canvas, startPos, sf::Mouse::getPosition(window));
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
        drawUI(window, penButtonBounds, rectangleButtonBounds, circleButtonBounds, undoButtonBounds, redoButtonBounds);

        window.display();
    }
}