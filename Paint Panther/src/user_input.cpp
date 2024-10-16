#include "../include/paint.hpp"

// handles mouse button release events
void handleMouseReleased(bool& isDrawingSquare, bool& isDrawingPixel) {
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        isDrawingPixel = false;
        isDrawingSquare = false;
    }
}

// handles mouse button press events
void handleMousePressed(sf::RenderWindow& window, bool& isDrawingrectangle, bool& isDrawingPixel,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds) {
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
        else if (currentTool == 1) {
            isDrawingPixel = true;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && currentTool == 2) {
        isDrawingrectangle = true;
        startPos = sf::Mouse::getPosition(window);
    }
}

// handles window events
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingrectangle, bool& isDrawingPixel,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds) {
    sf::Event event;
    if (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            handleMousePressed(window, isDrawingrectangle, isDrawingPixel, startPos, currentTool, penButtonBounds, rectangleButtonBounds);
            break;
        case sf::Event::MouseButtonReleased:
            handleMouseReleased(isDrawingrectangle, isDrawingPixel);
            break;
        default:
            break;
        }
    }
}
