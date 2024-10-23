#include "../include/paint.hpp"

// draws a pixel at the given mouse position
void drawPixel(sf::RenderTexture& canvas, const sf::Vector2f& mousePosition) {
    sf::CircleShape pixel(2);
    pixel.setPosition(mousePosition);
    pixel.setFillColor(sf::Color::Black);
    canvas.draw(pixel);
}

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

    UndoRedoManager undoRedoManager;

    while (window.isOpen()) {
        handleDrawingEvents(window, isDrawingRectangle, isDrawingPixel, isDrawingCircle, startPos, currentTool, penButtonBounds, rectangleButtonBounds, circleButtonBounds, undoButtonBounds, redoButtonBounds);

        // Save state before drawing
        if (isDrawingRectangle || isDrawingCircle || isDrawingPixel) {
            undoRedoManager.saveState(canvas);
        }

        // Draw rectangle
        if (isDrawingRectangle) {
            drawRectangle(canvas, startPos, sf::Mouse::getPosition(window));
        }

        // Draw circle
        if (isDrawingCircle) {
            drawCircle(canvas, startPos, sf::Mouse::getPosition(window));
        }

        // Draw pixel
        if (isDrawingPixel) {
            sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            drawPixel(canvas, mouseCoords);
        }

        // Handle undo/redo actions
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            undoRedoManager.undo(canvas);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
            undoRedoManager.redo(canvas);
        }

        // Draw canvas
        sf::Sprite canvasSprite(canvas.getTexture());
        window.draw(canvasSprite);
        canvas.display();

        // Draw UI
        drawUI(window, penButtonBounds, rectangleButtonBounds, circleButtonBounds, undoButtonBounds, redoButtonBounds);

        window.display();
    }
}