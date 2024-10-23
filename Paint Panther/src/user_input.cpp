#include "../include/paint.hpp"

// handles mouse button release events
// handles mouse button release events
void handleMouseReleased(bool& isDrawingRectangle, bool& isDrawingPixel, bool& isDrawingCircle) {
    isDrawingRectangle = false;
    isDrawingPixel = false;
    isDrawingCircle = false;
}

// handles mouse button press events
void handleMousePressed(sf::RenderWindow& window, bool& isDrawingRectangle, bool& isDrawingPixel, bool& isDrawingCircle,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds, const sf::FloatRect& circleButtonBounds,
    const sf::FloatRect& undoButtonBounds, const sf::FloatRect& redoButtonBounds) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (penButtonBounds.contains(mousePos)) {
            currentTool = 1; // Pen
            std::cout << "Pen tool selected" << std::endl;
        }
        else if (rectangleButtonBounds.contains(mousePos)) {
            currentTool = 2; // Rectangle
            std::cout << "Rectangle tool selected" << std::endl;
        }
        else if (circleButtonBounds.contains(mousePos)) {
            currentTool = 3; // Circle
            std::cout << "Circle tool selected" << std::endl;
        }
        else if (undoButtonBounds.contains(mousePos)) {
            // Perform undo action
            std::cout << "Undo action" << std::endl;
        }
        else if (redoButtonBounds.contains(mousePos)) {
            // Perform redo action
            std::cout << "Redo action" << std::endl;
        }
        else if (currentTool == 1) {
            isDrawingPixel = true;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (currentTool == 2) {
            isDrawingRectangle = true;
            startPos = sf::Mouse::getPosition(window);
        }
        else if (currentTool == 3) {
            isDrawingCircle = true;
            startPos = sf::Mouse::getPosition(window);
        }
    }
}

// handles window events
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingRectangle, bool& isDrawingPixel, bool& isDrawingCircle,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds, const sf::FloatRect& circleButtonBounds,
    const sf::FloatRect& undoButtonBounds, const sf::FloatRect& redoButtonBounds) {
    sf::Event event;
    if (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            handleMousePressed(window, isDrawingRectangle, isDrawingPixel, isDrawingCircle, startPos, currentTool, penButtonBounds, rectangleButtonBounds, circleButtonBounds, undoButtonBounds, redoButtonBounds);
            break;
        case sf::Event::MouseButtonReleased:
            handleMouseReleased(isDrawingRectangle, isDrawingPixel, isDrawingCircle);
            break;
        default:
            break;
        }
    }
}